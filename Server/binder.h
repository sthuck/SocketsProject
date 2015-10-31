#ifndef __BINDER_H
#define __BINDER_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
int do_bind_int(int port);
int do_bind(char* port);
#endif

