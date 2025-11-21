#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED













#include <stdint.h>





void* core_alloc(uint64_t size);
void core_free(void* mem);










#endif