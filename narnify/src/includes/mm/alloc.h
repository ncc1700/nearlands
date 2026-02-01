#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED







#include <types.h>


boolean MmInitGeneralAllocator();
void* MmAllocateGeneralMemory(u64 allocSize);
boolean MmFreeGeneralMemory(void* address);








#endif