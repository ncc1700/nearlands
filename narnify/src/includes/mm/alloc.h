#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED







#include <types.h>


NearStatus MmInitGeneralAllocator();
void* MmAllocateGeneralMemory(u64 allocSize);
void MmSetAllowFrees(boolean value);
NearStatus MmFreeGeneralMemory(void* address);








#endif