#ifndef VMM_H_INCLUDED
#define VMM_H_INCLUDED







#include <types.h>
#include <bootinfo.h>


#define PAGE_SIZE 0x1000
#define PG_PRESENT 0x1
#define PG_READ_WRITE 0x2
#define PG_PS 0x80

#define GET_PML4_ENTRY(x) ((x & ((u64)0x1ff << 39)) >> 39)
#define GET_PDPT_ENTRY(x) ((x & ((u64)0x1ff << 30)) >> 30)
#define GET_PD_ENTRY(x) ((x & ((u64)0x1ff << 21)) >> 21)
#define GET_PT_ENTRY(x) ((x & ((u64)0x1ff << 12)) >> 12)

#define PARSE_ENTRY_VALUE(x) (uptr*)(x & ~0xFFF)

#define HHDM_OFFSET 0xFFFFFF8000000000ULL

typedef struct _PageTable {
    __attribute__((aligned(4096))) uptr* pml4;
} PageTable;

boolean MmMapPage(PageTable* table, u64 physAddr, u64 virtAddr, u64 flags);
boolean MmUnmapPage(PageTable* table, u64 virtAddr);
boolean MmCheckIfPageIsMapped(PageTable* table, u64 virtAddr);
PageTable* MmReturnKernelPageTable();
boolean MmInitVirtualMemoryManager(BootInfo* info);
void MmUpdateCr3(u64 pml4Addr);
void MmInvalidatePage(u64 virtAddr);





















#endif