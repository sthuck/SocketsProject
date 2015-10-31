#ifndef __PORTALLOC_H
#define __PORTALLOC_H
#include <sys/types.h>

struct PortAllocator {
	int top;
	u_int16_t* arr;
	u_int16_t lower;
	u_int16_t upper;
};
typedef struct PortAllocator PortAllocator;

struct PortAllocator init_pa(u_int16_t lower, u_int16_t upper);
void relese_port(struct PortAllocator* allocator, short port);
u_int16_t alloc_port(struct PortAllocator* allocator);
void free_pa(struct PortAllocator* allocator);
int available_ports();
#endif

