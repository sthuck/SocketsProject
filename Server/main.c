#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include "HashTable.h"
#include "Protocol.h"
#include "binder.h"
#include "dbg.h"
#include "signal.h"
#define PORT "50006"

int do_bind(char* port);
char old_cwd[1024];
fd_set master;    // master file descriptor list
fd_set w_master;
int fdmax;

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) 
		return &(((struct sockaddr_in*)sa)->sin_addr);
		
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sig_handler(int signo) {
    chdir(old_cwd);
}

int main(int argc, char *argv[]) {
	if (argc!=2) {
		fprintf(stderr,"usage: multi_ftp_server <server_local_path>\n");
		exit(-1);
	}
	signal(SIGINT, sig_handler);
	getcwd(old_cwd,sizeof(old_cwd));
	if (chdir(argv[1])==-1) {
		log_err("Bad Directory:");
		exit(-1);
	}
	
    fd_set read_fds,write_fds;  // temp file descriptor list for select()
    int listener;     // listening socket descriptor
    FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&w_master);
    FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	init_protocol();
	
	// get us a socket and bind it
	listener = do_bind(PORT);
    FD_SET(listener, &master);
	fdmax = listener + 1;
	for(;;) {
        read_fds = master; // copy it
		write_fds = w_master;
        if (select(fdmax+1, &read_fds, &write_fds, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        // run through the existing connections looking for data to read
		int fdmax_nochange = fdmax;
        for(int i = 0; i <= fdmax_nochange; i++) {
			//debug("fdmax is %d",fdmax_nochange);
            if (FD_ISSET(i, &read_fds)) {
                if (i == listener) {
					fdmax = handle_new_client(listener) ; 
                } 
				else {
					fdmax = handle_recv_from_client(i);  // handle data from a client
                }
            }
			
			if (FD_ISSET(i,&write_fds)) {
				fdmax = handle_send_to_client(i);
			}
        }//end run thrugh read fd_set
        
    }
    
}

