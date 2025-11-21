#ifndef PHYSMEM_H_INCLUDED
#define PHYSMEM_H_INCLUDED







#include "../../ldrconfig.h"



void initialize_physmem_allocator(MemoryMap* memmap, uint64_t memory_amount);
void* physmem_allocate_page();
void* physmem_allocate_multiple_pages(uint64_t amount);

void physmem_free_page(void* mem);
void physmem_free_multiple_pages(void* mem, uint64_t amount);



#endif
