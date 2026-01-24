#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED









#include "../../types.h"




boolean MmInitHeapAllocator(u64 amountInPages);
void* MmAllocateFromHeap(u16 size);
boolean MmFreeFromHeap(void* address, u16 size);















#endif