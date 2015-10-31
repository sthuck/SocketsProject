#include <stdlib.h>
#include "portAlloc.h"
#include <time.h>


struct PortAllocator init_pa(u_int16_t lower, u_int16_t upper) {
	srand ( time(NULL) );
	struct PortAllocator res;
	if (lower==0 || lower>upper)
		{
			//needs error handling
			return res;
		}

	res.upper=upper;
	res.lower=lower;
	res.top=upper-lower;
	res.arr=malloc(res.top*sizeof(short));
	for (int i=0;i<res.top;i++)
		res.arr[i]=lower+i;
	return res;
}

u_int16_t alloc_port(struct PortAllocator* allocator) {
	if (allocator->top==0)
		return 0;
	
	int index = rand() % allocator->top--;
	short temp = allocator->arr[index];
	allocator->arr[index]=allocator->arr[allocator->top];
	allocator->arr[allocator->top]=temp;
	return temp;
}

void relese_port(struct PortAllocator* allocator, short port) {
	allocator->arr[allocator->top++]=port;
}

void free_pa(struct PortAllocator* allocator) {
	free(allocator->arr);
}
int available_ports(struct PortAllocator* allocator) {
	return allocator->top;

}
