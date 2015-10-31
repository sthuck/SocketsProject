#include <stdlib.h>
#include <stdio.h>
#include "HashTable.h"

int main() {
	hash_table ht;
	init_ht(&ht);
	add_filecopy_ht(&ht,5);
	add_conn_ht(&ht,4);
	free_ht(&ht);
	entry_header* eh = get_ht(&ht,4);
	entry_header* eh2 = get_ht(&ht,5);
	printf("%d\n",eh->type);
	printf("%d\n",eh2->type);
	remove_ht(&ht,4);
	remove_ht(&ht,5);
	free_ht(&ht);
}