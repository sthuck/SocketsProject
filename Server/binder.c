#include "binder.h"
#include "dbg.h"
/*
 * Does the 'ugly' work of binding a TCP port
 * returns socket descriptor of listening port
 */
int do_bind(char* port) {
	int listener;
	struct addrinfo hints, *ai,*p;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	int rv;
	if ((rv = getaddrinfo(NULL, port, &hints, &ai)) != 0) {
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}
	
	for(p = ai; p != NULL; p = p->ai_next) {
    	listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		fcntl(listener, F_SETFL, O_NONBLOCK);
		if (listener < 0) { 
			continue;
		}
		
		// lose the pesky "address already in use" error message
		int yes=1;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(listener);
			continue;
		}
		debug("listening on port %s",port);
		break;
	}

	// if we got here, it means we didn't get bound
	if (p == NULL) {
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}
	freeaddrinfo(ai);
	if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }
	return listener;
}
int do_bind_int(int port) {
	char buf[6];
	//sprintf leads to a nasty bug
	snprintf(buf,6,"%d",port);
	return do_bind(buf);
}
