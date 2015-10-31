#ifndef __DS_H
#define __DS_H
#include <stdlib.h>
#include <sys/types.h>
#include "List.h"
#define HT_BUCKETS 256
#include <stdlib.h>
#define BUF_SIZE 512

typedef enum {FILE_SEND} type_e;


typedef enum {SEND_FILESIZE,SEND_FILENAME_LEN,SEND_FILENAME,SEND_DATA} stage_e;


struct entry_header {
	struct list_head list;
	int key; //fd
	type_e type;
	stage_e stage;
};
typedef  struct entry_header entry_header;
struct entry_send {
	struct list_head list;
	int key; //fd
	type_e type;
	stage_e stage;
	char* filename;
	int size;
	int diskfile_fd;
	int data_sent;
	char databuf[BUF_SIZE];
	ssize_t buf_pos;
	ssize_t buf_end;
};
typedef struct entry_send entry_send;

struct hash_table {
	struct list_head storage[HT_BUCKETS];
	int num_objects;	
};
typedef struct hash_table hash_table;

void init_ht(hash_table* ht);
void free_ht(hash_table* ht);
void add_filesend_ht(hash_table* ht, int fd,char* filename, int diskfile_fd, int size);
entry_header* get_ht(hash_table* ht, u_int32_t key);
void remove_ht(hash_table *ht, u_int32_t key);


#endif

