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

void 
add_conn_ht(hash_table* ht, int fd) {
	entry_conn* ec = (entry_conn*)malloc(sizeof(entry_conn));
	ec->key=fd;
	ec->type=CONNECTION;
	ec->stage=RCV_FILE_NUM; //inital stage
	ec->num_of_files=0;
	INIT_LIST_HEAD(&ec->list);
	int index = fd%HT_BUCKETS;
	list_add(&ht->storage[index],&ec->list);
	ht->num_objects++;
}

void 
add_filecopy_ht(hash_table* ht, int fd, int conn_fd) {
	entry_filecopy* fc = (entry_filecopy*)malloc(sizeof(entry_filecopy));
	fc->key = fd;
	fc->type = FILECOPY;
	fc->rcvd = 0;
	fc->stage=RCV_FILESIZE; //inital stage
	fc->conn_fd = conn_fd;
	INIT_LIST_HEAD(&fc->list);
	int index = fd%HT_BUCKETS;
	list_add(&ht->storage[index],&fc->list);
	ht->num_objects++;
}

void 
add_filecopy_listen(hash_table* ht, int fd, int port, int conn_fd) {
	entry_filecopy_listen* efc = (entry_filecopy_listen*)malloc(sizeof(entry_filecopy_listen));
	efc->key = fd;
	efc->type = FILECOPY_LISTEN;
	efc->stage = ACCEPT_CONN; //inital stage
	efc->conn_fd = conn_fd;
	efc->port=port;
	INIT_LIST_HEAD(&efc->list);
	int index = fd%HT_BUCKETS;
	list_add(&ht->storage[index],&efc->list);
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

