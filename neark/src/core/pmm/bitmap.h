#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED





#include <stdint.h>
#include <stddef.h>





#include "../../ldrconfig.h"


void bmp_init(MemoryMap* memmap);
void* bmp_allocate_pages(uint64_t pages);
void bmp_free_pages(void* mem, uint64_t pages);









#endif
