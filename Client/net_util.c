/**
 * @brief casts a sockaddr to the ipv4 struct or ipv6
 * @param sa
 */
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


/**
 * @brief 
 * @param path - to look for files
 * @param file_count changes this to number of files in dir
 * @return array of filenames
 */
char** get_files(int* file_count) { 
	DIR* curr = opendir("./");
	char** files;
	if (curr==NULL) {
		log_err("Can't find Path.");
		_exit(-1);
	}
	*file_count=0;
	struct dirent* entry;
	
	while ((entry = readdir(curr)) != NULL && *file_count<5000) {
		if (entry->d_type == DT_REG) { /* If the entry is a regular file */
			 (*file_count)++;
		}
	}
	printf("file count %d\n",*file_count);
	closedir(curr);
	files = (char**)malloc(sizeof(char*)**file_count);
	
	curr = opendir("./");
	int i=0;
	while ((entry = readdir(curr)) != NULL && i<5000) {
		if (entry->d_type == DT_REG) { /* If the entry is a regular file */
			files[i]=(char*)malloc(FILENAME_MAX*sizeof(char));
			memcpy(files[i],entry->d_name,FILENAME_MAX);
			i++;
		}
	}
	closedir(curr);
	return files;
}


void free_files(int n ,char** files) { 
	for (int i=0;i<n;i++) 
		free(files[i]);
		free (files);
} 


/**
 * @brief connects to host and port
 * @param host - 
 * @param port - 
 * @return socket desc
 */
int connect_port(char* host, char* port) {
	struct addrinfo hints, *servinfo, *p;
	int rv,sockfd;
	memset(&hints, 0, sizeof hints);
	char s[INET6_ADDRSTRLEN];

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        log_err("getaddrinfo: %s\n", gai_strerror(rv));
        _exit(-1);
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            log_err("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            log_err("client: connect");
            continue;
        }

        break;
    }
	//got all the way here without connecting...
    if (p == NULL) {
        log_err("client: failed to connect\n");
		freeaddrinfo(servinfo);
        return -1;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    debug("client: connecting to %s::%s", s,port);
	freeaddrinfo(servinfo);
	return sockfd;
}

int connect_port_int(char*host,int port) {
	char buf[6];
	snprintf(buf,6,"%d",port);
	return connect_port(host,buf);
}