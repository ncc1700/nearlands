#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED







#include <stdint.h>
#include <stddef.h>

uint8_t bitmap_init();
void* allocate_single_bitmap();
void free_single_bitmap(void* address);
void* allocate_multiple_bitmaps(uint64_t amount);
void free_multiple_bitmaps(void* address, uint64_t amount);

#endif