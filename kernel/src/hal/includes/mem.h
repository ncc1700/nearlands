#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED







#include <stdint.h>
#include <stddef.h>

uint8_t chunk_allocator_setup();
void* allocate_single_chunk();
void free_single_chunk(void* address);
void* allocate_multiple_chunks(uint64_t amount);
void free_multiple_chunks(void* address, uint64_t amount);


#endif