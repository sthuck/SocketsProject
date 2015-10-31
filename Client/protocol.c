#include <sys/socket.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "dbg.h"
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "HashTable.h"
#include "net_util.c"
extern hash_table ht;


/**
 * @brief recives port numbers from server
 * @param sd - socket desc
 * @param ports - int array to store ports
 * @param num_files
 * @return 0 on success, -1 on fail
 */
int get_ports(int sd, int* ports, int num_files) {
	int numbytes;
	for (int i=0;i<num_files;i++) {
		if ((numbytes = recv(sd, &ports[i], sizeof(int), 0)) < sizeof(int)) {
        perror("recv");
        return -1;
		}
	}
	return 0;
}


/**
 * @brief sets up transfer of files. creates object for each file+port, 
 * puts in global hash_table and in 'master' fd_set.
 * @param ports array of available ports at host
 * @param files array of filenames to copy.
 * @param num_files
 * @param host host to connect to.
 * @param master
 * @param fdmax - changes fdmax
 * @return number of actual files we want to send. Could be less than num_files in case of errors.
 */
 
int send_files_to_set(int* ports,char** files, int num_files, 
			char* host, fd_set* master,int* fdmax) {
	int files_to_copy=0; //different counter, as we might not copy all files
	int newfd;
	for (int i=0;i<num_files;i++) {
		if ((newfd = connect_port_int(host,ports[i])) == -1) {
			log_err("failed to connect to file transfer port: %d",ports[i]);
			i--;
			sleep(1);
			continue;
		}
		int diskfile_fd;
		if ((diskfile_fd =open(files[i],O_RDONLY)) == -1)  {
			log_err("can't open file %s, permissions?",files[i]);
			continue;
		}
		FD_SET(newfd,master);
		struct stat statbuf;
		fstat(diskfile_fd,&statbuf);
		int filesize = statbuf.st_size;
		add_filesend_ht(&ht,newfd,files[i],diskfile_fd,filesize);
		files_to_copy++;
		*fdmax=newfd; //last fd is max
	}
	return files_to_copy; 
}

int send_filesize(entry_send* es) {
	if (send(es->key,&es->size,sizeof(es->size),0)==-1) {
		log_err("error in send filesize for file %s",es->filename);
		return -1;
	}
	es->stage++;
	return 0;
}

int send_filename_len(entry_send* es) {
	int length = strlen(es->filename);
	debug("sedning filename len: %d",length);
	if (send(es->key,&length,sizeof(length),0)==-1) {
		log_err("error in send filename length for file %s",es->filename);
		return -1;
	}
	es->stage++;
	return 0;
}

int send_filename(entry_send* es) {
	int length = strlen(es->filename);
	debug("sedning file name: %s",es->filename);
	if (send(es->key,es->filename,length,0)<length) {
		log_err("error in send filename for file %s",es->filename);
		return -1;
	}
	es->stage++;
	return 0;
}

int send_data(entry_send* es) {
	if (es->buf_pos==BUF_SIZE) {  //do new read
		es->buf_pos=0;
		int nbytes;
		if ((nbytes = read(es->diskfile_fd,es->databuf,BUF_SIZE)) == -1) {
			log_err("error reading from disk, file: %s",es->filename);
			return nbytes;
		}
		es->buf_end=nbytes;
	}
	  //send old read or the read we just did;
	int rv;
	rv = send(es->key,es->databuf+es->buf_pos,es->buf_end-es->buf_pos,0); 
	if (rv==-1)
		log_err("error sending data");
	es->buf_pos+=rv;
	es->data_sent+=rv;
	return rv;
}

int(*do_action[4])(entry_send* es) = 
					{send_filesize, send_filename_len, send_filename,send_data};

void close_connection(entry_send* es, fd_set* master) {
	close(es->key);
	FD_CLR(es->key,master);
	remove_ht(&ht,es->key);
}
void dispatch(int fd, int* to_copy, fd_set* master) {
	entry_send* es = (entry_send*)get_ht(&ht,fd);
	int rv = do_action[es->stage](es);
	if (rv == -1 || es->data_sent>=es->size) {
		(*to_copy)--;
		close_connection(es,master);
	}
}
