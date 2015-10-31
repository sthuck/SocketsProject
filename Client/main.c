#define PORT "50006"
#include "protocol.c"
#include "signal.h"

hash_table ht;
fd_set master;
int sd=-1;
char old_cwd[1024];
char** files = NULL;
int num_files=0;
int* ports = NULL;

void _exit(int status) {
	if (sd!=-1)
		close(sd);
	if (files)
		free_files(num_files,files);
	if (ports)
		free(ports);
	chdir(old_cwd);
	exit(status);
}

void sig_handler(int signo) {
    chdir(old_cwd);
}

int main(int argc, char **argv)
{
	if (argc!=3) {
		fprintf(stderr,"usage: multi_ftp_client <server_address> <client_local_path>\n");
		exit(-1);
	}
	getcwd(old_cwd,sizeof(old_cwd));
	if (chdir(argv[2])==-1) {
		log_err("Bad Directory:");
		_exit(-1);
	}
	
	init_ht(&ht);
	files = get_files(&num_files);  
	ports = (int*)malloc(num_files*sizeof(int));
	int sd = connect_port(argv[1],PORT);
	
	if (send(sd,&num_files,sizeof(int),0)==-1) {
		log_err("error in send");
		_exit(-1);
	}
	if (get_ports(sd,ports,num_files)==-1) {
		log_err("error in getting ports");
		_exit(-1);
	}
	
	for (int i=0;i<num_files;i++) {
		printf("%d\n",ports[i]);
	}
	
	int fdmax;
	int files_to_copy=send_files_to_set(ports,files,num_files,argv[1],&master,&fdmax);
	
	//big select loop
	for(;files_to_copy>0;) {
		//debug("files to copy: %d", files_to_copy);
        fd_set write_fds = master; // copy it
		if (select(fdmax+1, NULL,&write_fds, NULL, NULL) == -1) {
            log_err("select");
            exit(4);
        }
		for(int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &write_fds)) { //found one ready to write
				dispatch(i,&files_to_copy, &master);
			} 
		}
	}
	
	_exit(0);
}
	


