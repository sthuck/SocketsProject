#ifndef __DS_H
#define __DS_H
#include <stdlib.h>
#include <sys/types.h>
#include "List.h"
#define HT_BUCKETS 256
#include <stdlib.h>
#include "portAlloc.h"

typedef enum {CONNECTION, FILECOPY,FILECOPY_LISTEN} type_e;


typedef enum {RCV_FILE_NUM,SEND_PORTS,
	ACCEPT_CONN,
	RCV_FILESIZE,RCV_FILENAME_LEN,RCV_FILENAME,OPEN_DISK_FD,RCV_DATA,CLOSE} stage_e;


struct entry_header {
	struct list_head list;
	int key; //fd
	type_e type;
	stage_e stage;
};
typedef  struct entry_header entry_header;

struct entry_conn {
	struct list_head list;
	int key; //fd
	type_e type;
	stage_e stage;
	int num_of_files;
	int* fds;  //variable length; &fds[i];
	int* filefds;
	};
typedef struct entry_conn entry_conn;

struct entry_filecopy_listen {
	struct list_head list;
	int key; //fd
	type_e type;
	stage_e stage;
	int port;
	int conn_fd;
	int copy_fd;
};
typedef struct entry_filecopy_listen entry_filecopy_listen;

struct entry_filecopy {
	struct list_head list;
	int key; 
	type_e type;
	stage_e stage;
	int filename_length;	
	char filename[256];
	int size;
	int rcvd;
	int conn_fd;
	int file_fd;
};
typedef struct entry_filecopy entry_filecopy;

struct hash_table {
	struct list_head storage[HT_BUCKETS];
	int num_objects;	
};
typedef struct hash_table hash_table;

void init_ht(hash_table* ht);
void free_ht(hash_table* ht);
void add_conn_ht(hash_table* ht, int fd);
void add_filecopy_ht(hash_table* ht, int fd,int conn_fd);
void add_filecopy_listen(hash_table* ht, int fd,int port, int conn_fd);
entry_header* get_ht(hash_table* ht, u_int32_t key);
void remove_ht(hash_table *ht, u_int32_t key);


#endif

