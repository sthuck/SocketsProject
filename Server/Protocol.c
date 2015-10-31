#include "Protocol.h"
#include "dbg.h"
#define lowerb 60000
#define upperb 65000

extern fd_set w_master;
extern fd_set master;
extern int fdmax;

static hash_table ht;
static struct PortAllocator pa;
static int(*do_action[10])(entry_header* eh) = {
	rcv_file_num,send_ports,accept_filetransfer,rcv_file_size,rcv_filename_len,rcv_filename,open_disk_fd,rcv_data,close_conn
};

static inline int read_sd(int sd, void* buf, int size) {
	int nbytes;
	if ((nbytes = recv(sd, buf, size, 0)) <= 0) {
		// got error or connection closed by client
		if (nbytes != 0) 
			log_err("error in recv, sd: %d",sd);
	} 
	return nbytes;
}

void init_protocol() {
	init_ht(&ht);
	pa = init_pa(lowerb,upperb);
}
/*
 *returns new fdmax 
 */
int handle_new_client(int listener) {
	int newfd = accept(listener,NULL,NULL);	
	if (newfd == -1) {
		perror("accept");
	} else {
		FD_SET(newfd, &master); // add to master set
		if (newfd > fdmax) {    
			fdmax = newfd;
		}
	add_conn_ht(&ht,newfd);    //add connection to table
	}
	return fdmax;
}

int handle_recv_from_client(int fd) {
	entry_header* eh = get_ht(&ht,fd);
		if (eh==NULL) {
			log_err("handle_recv_from_client => got fd not registerd: %d",fd);
			return fdmax;
		}
	debug("got data on fd: %d", eh->key);
								// we got some data from a client
	return do_action[eh->stage](eh);
	  	
}

int handle_send_to_client(int fd) {
	entry_header* eh = get_ht(&ht,fd);
		if (eh==NULL) {
			log_err("handle_recv_from_client => got fd not registerd: %d",fd);
			return fdmax;
		}
	debug("ready to write data on fd: %d", eh->key);
	return do_action[eh->stage](eh);
}


/***********************/

/**
 * Closes the connection.
 * If it's a CONNECTION type, closes every listening port also.
 * removes it from 'ht' table. removes from fd_set master and w_master.
 * closes socekt descriptor.
 * 
 */ 
static int handle_close_connection_eh(entry_header* eh) {
	int fd = eh->key;
	if (eh->type==CONNECTION) {
		entry_conn* ec = (entry_conn*) eh;
		for (int i=0;i<ec->num_of_files;++i) {       	//closing all listening ports, in case of crash on client
			entry_filecopy_listen* efc = (entry_filecopy_listen*)get_ht(&ht, ec->fds[i]);
			if (efc!=NULL && efc->conn_fd==fd) {   		//checking if it's our 'child'
				fdmax = handle_close_connection(efc->key);
			}
		}
		free(ec->fds);
		free(ec->filefds);
	}
	else if (eh->type==FILECOPY_LISTEN) {
		entry_filecopy_listen* efc = (entry_filecopy_listen*)eh;
		relese_port(&pa,efc->port);
	}
	else {   			//FILECOPY type
		entry_filecopy* fc = (entry_filecopy*)eh;
		close(fc->file_fd);
	}
	
	//do for all types of entries;
	close(eh->key);
	FD_CLR(eh->key,&master);
	FD_CLR(eh->key,&w_master);
	remove_ht(&ht,fd);
	if (fd==fdmax)
		fdmax-=1;
	return fdmax;
}
int handle_close_connection(int fd) {
	entry_header* eh = get_ht(&ht, fd);
	return handle_close_connection_eh(eh);
}
		
int rcv_file_num(entry_header* eh) {
	entry_conn* ec = (entry_conn*)eh;
	int nbytes = read_sd(ec->key,&ec->num_of_files,sizeof(ec->num_of_files));
	if (nbytes<=0) {
		handle_close_connection_eh(eh);
		return fdmax;
	}
	debug("debug:recived number of files %d\n",ec->num_of_files);
	ec->stage++;
	return send_ports(eh);	
}

int rcv_file_size(entry_header* eh){
	entry_filecopy* ef = (entry_filecopy*)eh;
	if (read_sd(ef->key,&ef->size,sizeof(int))<sizeof(int)) {
		return handle_close_connection_eh(eh);
	}
	else {
		debug("debug:recived filesize: %d\n",ef->size);
		ef->stage=RCV_FILENAME_LEN;     //setting next stage
	}
	return fdmax;
}

int rcv_filename_len(entry_header* eh){
	entry_filecopy* ef = (entry_filecopy*)eh;
	if (read_sd(ef->key,&ef->filename_length,sizeof(int))<sizeof(int)) {
		log_err("rcv_filename_len:got insufficent bytes");
		return handle_close_connection_eh(eh);
	}
	else {
		debug("debug:recived filename length: %d\n",ef->filename_length);
		memset(ef->filename,0,256); //zeroing filename bufffer;
		ef->stage=RCV_FILENAME;     //setting next stage
	}
	return fdmax;
}


int rcv_filename(entry_header* eh){
	entry_filecopy* ef = (entry_filecopy*)eh;
	int nbytes = read_sd(ef->key,ef->filename,ef->filename_length);
	if (nbytes<0) {
		handle_close_connection_eh(eh);
	}

	ef->stage=OPEN_DISK_FD;
	return fdmax;
}

int open_disk_fd(entry_header* eh) {
	entry_filecopy* ef = (entry_filecopy*)eh;
	struct stat buffer;
	if (stat(ef->filename,&buffer)==0) {                   //if file exsits
		log_err("file already exsits! %s",ef->filename);
		fdmax = handle_close_connection_eh(eh);
		return fdmax;
	}
	int filefd = open(ef->filename,O_RDWR|O_CREAT);
	if (filefd==-1) {
		log_err("error creating file named: %s",ef->filename);
		fdmax = handle_close_connection_eh(eh);
		return fdmax;
	}
	else {
		ef->file_fd=filefd;
		ef->rcvd=0;
		ef->stage=RCV_DATA;
		fdmax = rcv_data(eh);
	}
	return fdmax;
}

int rcv_data(entry_header* eh) {
	entry_filecopy* ef = (entry_filecopy*)eh;
	char buf[BUF_SIZE];
	int nbytes = read_sd(ef->key,&buf,BUF_SIZE);
	if (nbytes<=0) {
		 return  handle_close_connection_eh(eh);
	}
	
	write(ef->file_fd,buf,nbytes);   //write to actual file
	ef->rcvd+=nbytes;
	
	if (ef->rcvd>=ef->size) {  		//finished reading
		close(ef->file_fd);
		log_info("Finished getting file: %s",ef->filename);
		fdmax = handle_close_connection_eh(eh);
	}
	return fdmax;
}

/**
 *Closes the connection.
 *Unlike the handle_close_connection() functions -
 *This is meant as a 'natural' close connection, not for errors.
 */
int close_conn(entry_header* eh) {
	char buf[BUF_SIZE];
	int nbytes = read_sd(eh->key,&buf,BUF_SIZE);
	if (nbytes>0)
		debug("Was supposed to receive close connection, but got data! bytes: %d fd: %d",nbytes, eh->key);
	return handle_close_connection_eh(eh);
}


/**
 * Gets a filecopy_listen type - 
 * accepts the connectionf.
 * adds new fd to tables (ht, fd_set master)
 * closes listening port.
 */
int accept_filetransfer(entry_header* eh) {
	if (eh->type!=FILECOPY_LISTEN) {
		log_err("accept_filtransfer:got bad entry header");
		return fdmax;
	}
	entry_filecopy_listen* efc = (entry_filecopy_listen*) eh;
	//should have code to check if incoming connection is from the same client we expect it to be
	int newfd = accept(efc->key,NULL,NULL);
	debug("accepted new filecopy connection on port %d",efc->port);
	add_filecopy_ht(&ht,newfd,efc->conn_fd);
	FD_SET(newfd,&master);
	if (newfd>fdmax)
		fdmax=newfd;
	fdmax = handle_close_connection_eh(eh);  //closing this listening port! 
	return fdmax;
}


/*
 * send port numbers to client.
 * opens new listening ports.
 * reruns the new fdmax.
 */
int send_ports(entry_header* eh) {
	entry_conn* ec = (entry_conn*)eh;
	FD_SET(ec->key,&w_master);
	int fd=ec->key;
	int avail_ports = available_ports(&pa);
	if (avail_ports<ec->num_of_files) {
		log_info("all out of ports, waiting...");
		return fdmax;
	}
	ec->fds = (int*)calloc(ec->num_of_files,sizeof(int));
	ec->filefds = (int*)calloc(ec->num_of_files,sizeof(int));
	for (int i = 0;i<ec->num_of_files;i++) {
		if (ec->fds[i]!=0) 								//fds[i] could be full if we stopped during previous right
			continue;
		int port = alloc_port(&pa);
		int rv = send(fd,&port,sizeof(int),0); 			//if send fails it's ok. problem is when it sends less than 4 bytes
		if (rv<sizeof(int)) {
			log_err("send failed, trying again later");
			return fdmax;
		}
		ec->fds[i] = do_bind_int(port); 				//saving new socket descriptor
		add_filecopy_listen(&ht, ec->fds[i], port, ec->key);
		debug("opend new fd for listening port: %d",ec->fds[i]);
		FD_SET(ec->fds[i],&master);
		if (ec->fds[i]>fdmax)
			fdmax=ec->fds[i];
	}
	ec->stage=CLOSE;
	FD_CLR(ec->key,&w_master);
	return fdmax;
}

