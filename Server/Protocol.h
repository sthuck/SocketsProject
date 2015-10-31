#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include "HashTable.h"
#include "portAlloc.h"
#include "binder.h"
#include "sys/stat.h"
#define BUF_SIZE 512 

void init_protocol();
int handle_recv_from_client(int fd);
int handle_new_client(int listener);
int handle_close_connection(int fd);
int handle_send_to_client(int fd);

int rcv_file_num(entry_header* eh);
int rcv_filename_len(entry_header* eh);
int rcv_filename(entry_header* eh);
int rcv_file_size(entry_header* eh);
int rcv_data(entry_header* eh);
int open_disk_fd(entry_header* eh);
int accept_filetransfer(entry_header* eh);
int close_conn(entry_header* eh);
int send_ports(entry_header* eh);

#endif

