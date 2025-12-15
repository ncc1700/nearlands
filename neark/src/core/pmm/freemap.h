#ifndef PHYSMEM_H_INCLUDED
#define PHYSMEM_H_INCLUDED







#include "../../ldrconfig.h"



void fmp_init(MemoryMap* memmap);
void* fmp_allocate_page();
void* fmp_allocate_multiple_pages(uint64_t amount);
void fmp_free_page(void* mem);
void fmp_free_multiple_pages(void* mem, uint64_t amount);


#endif
