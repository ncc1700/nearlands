#ifndef VMM_H_INCLUDED
#define VMM_H_INCLUDED







#include <types.h>
#include <bootinfo.h>


#define PAGE_SIZE 0x1000

#define HHDM_OFFSET 0xFFFFFF8000000000ULL

typedef struct _PageTable {
    __attribute__((aligned(4096))) uptr* pml4;
} PageTable;

boolean MmMapPage(PageTable* table, u64 physAddr, u64 virtAddr, u64 flags);
boolean MmInitVirtualMemoryManager(BootInfo* info);
void MmUpdateCr3(u64 pml4Addr);





















#endif