#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED







#include <stdint.h>
#include <stddef.h>
#include "../../limine.h"
uint8_t map_init();
void* allocate_single_map();
void free_single_map(void* address);
void* allocate_multiple_maps(uint64_t amount);
void free_multiple_maps(void* address, uint64_t amount);
struct limine_memmap_response* get_memmap();
struct limine_hhdm_response* get_hhdm();
#endif