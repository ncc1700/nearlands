#ifndef PMM_H_INCLUDED
#define PMM_H_INCLUDED







#include "../../types.h"
#include "../../bootinfo.h"

#define PAGE_SIZE 0x1000

#define PAGEMAP_PMM



#ifdef PAGEMAP_PMM
typedef struct _PageMap {
    u64 address;
    boolean isFree:1;
}  __attribute__((packed)) PageMap;
#endif
// #define BITMAP_PMM


boolean MmInitPhysicalMemoryManager(BootMemoryMap* memMap);
void* MmAllocateSinglePage();
void* MmAllocateMultiplePages(u64 amount);
boolean MmFreeSinglePage(void* pageAddress);
boolean MmFreeMultiplePages(void* pageAddress, u64 amount);
boolean MmTestPhysicalMemoryManager();
u64 MmReturnPageAmount();






#endif