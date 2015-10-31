#include "HashTable.h"
#include <stdio.h>


void 
init_ht(hash_table* ht) {
	for(int i=0;i<HT_BUCKETS;i++)
		INIT_LIST_HEAD(&ht->storage[i]);
	ht->num_objects=0;
}

void 
free_ht(hash_table* ht) {
	if (ht->num_objects>0)
		fprintf(stderr,"descriptor table not empty!\n");
}

void add_filesend_ht(hash_table* ht, int fd, char* filename, int diskfile_fd, int size) {
	entry_send* es = (entry_send*)malloc(sizeof(entry_send));
	es->key=fd;
	es->type=FILE_SEND;
	es->stage=SEND_FILESIZE; //inital stage
	es->data_sent=0;
	es->filename=filename;
	es->diskfile_fd=diskfile_fd;
	es->size=size;
	es->buf_pos=BUF_SIZE;
	es->buf_end=0;
	
	INIT_LIST_HEAD(&es->list);
	int index = fd%HT_BUCKETS;
	list_add(&ht->storage[index],&es->list);
	ht->num_objects++;
}

 
void 
remove_ht(hash_table *ht, u_int32_t key) {
	int index = key%HT_BUCKETS;
	struct entry_header* iterator;
	list_for_each_entry (iterator, &ht->storage[index], list) {
		if (iterator->key==key) {
			list_del(&iterator->list);
				free(iterator);
			--ht->num_objects;
			return;
		}
	}
	fprintf(stderr,"fd %d doesnt exsits in table",key);
}


/**
 * return null if not found.
 */
entry_header* 
get_ht(hash_table *ht, u_int32_t key) {
	u_int32_t index = key%HT_BUCKETS;
	struct entry_header* iterator;
	list_for_each_entry (iterator, &ht->storage[index], list) {
		if (iterator->key==key)
			return iterator;
	}
	return NULL; 
}

