#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED







#include <types.h>

#define USE_HOMEMADE_ALLOC
//#define USE_LIBALLOC
//#define USE_LEGACY_ALLOC
NearStatus MmInitGeneralAllocator();
void* MmAllocateGeneralMemory(u64 allocSize);
NearStatus MmFreeGeneralMemory(void* address);







#endif