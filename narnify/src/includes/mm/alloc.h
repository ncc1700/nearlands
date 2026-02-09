#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED







#include <types.h>

//#define USE_HOMEMADE
#define USE_LIBALLOC

NearStatus MmInitGeneralAllocator();
void* MmAllocateGeneralMemory(u64 allocSize);
void MmSetAllowFrees(boolean value);
NearStatus MmFreeGeneralMemory(void* address, u64 size);







#endif