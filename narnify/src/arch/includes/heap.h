#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED









#include "../../types.h"




boolean MmInitHeapAllocator(u64 heapSize);
void* MmAllocateFromHeap(u64 size);
boolean MmFreeFromHeap(void* address, u64 size);















#endif