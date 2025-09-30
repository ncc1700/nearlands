#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED







#include <stdint.h>
#include <stddef.h>

uint8_t chunk_allocator_setup();
void* allocate_single_chunk();
void free_single_chunk(void* address);




#endif