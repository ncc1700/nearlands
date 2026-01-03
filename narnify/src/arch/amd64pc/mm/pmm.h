#ifndef PMM_H_INCLUDED
#define PMM_H_INCLUDED







#include "../../../types.h"
#include "../../../bootinfo.h"





boolean MmInitPhysicalMemoryManager(BootMemoryMap* memMap);
void* MmAllocateSinglePage();
void* MmAllocateMultiplePages(u64 amount);
boolean MmFreeSinglePage(void* pageAddress);
boolean MmFreeMultiplePages(void* pageAddress, u64 amount);









#endif