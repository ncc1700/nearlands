#include <mm/vmm.h>
#include <mm/pmm.h>
#include <ke/graphics.h>
#include <qol/qmem.h>


static PageTable kernPTable = {NULL};


#define PG_PRESENT 0x1
#define PG_READ_WRITE 0x2
#define PG_PS 0x80

#define GET_PML4_ENTRY(x) ((x & ((u64)0x1ff << 39)) >> 39)
#define GET_PDPT_ENTRY(x) ((x & ((u64)0x1ff << 30)) >> 30)
#define GET_PD_ENTRY(x) ((x & ((u64)0x1ff << 21)) >> 21)
#define GET_PT_ENTRY(x) ((x & ((u64)0x1ff << 12)) >> 12)

#define PARSE_ENTRY_VALUE(x) (uptr*)(x & ~0xFFF)

#ifdef PAGEMAP_PMM
extern PageMap* MmReturnPageMap();
#endif

static inline boolean GetNextLevel(uptr* pml, u64 entry, u64 flags){
    if(entry >= 512) return FALSE;
    uptr* address = (uptr*)MmAllocateSinglePage();
    
    if(address == NULL){
        return FALSE;
    }
    if((uptr)address == 0x0) return FALSE;
    memset(address, 0, PAGE_SIZE);
    pml[entry] = (uptr)address | PG_PRESENT | PG_READ_WRITE;
    
    return TRUE;
}


boolean MmMapPage(PageTable* table, u64 physAddr, u64 virtAddr, u64 flags){
    u64 pml4Entry = GET_PML4_ENTRY(virtAddr);
    u64 pdptEntry = GET_PDPT_ENTRY(virtAddr);
    u64 pdEntry = GET_PD_ENTRY(virtAddr);
    u64 ptEntry = GET_PT_ENTRY(virtAddr);
    if(table->pml4[pml4Entry] == 0x0){
        boolean result = GetNextLevel(table->pml4, pml4Entry, flags | PG_READ_WRITE);
        if(result == FALSE){
            return FALSE;
        }
    }
    uptr* pdpt = PARSE_ENTRY_VALUE(table->pml4[pml4Entry]);
    if(pdpt[pdptEntry] == 0x0){
        boolean result = GetNextLevel(pdpt, pdptEntry, flags | PG_READ_WRITE);
        if(result == FALSE){
            return FALSE;
        }
    }
    uptr* pd = PARSE_ENTRY_VALUE(pdpt[pdptEntry]);
    if(pd[pdEntry] == 0x0){
        boolean result = GetNextLevel(pd, pdEntry, flags);
        if(result == FALSE){
            return FALSE;
        }
    }
    uptr* pt = PARSE_ENTRY_VALUE(pd[pdEntry]);
    pt[ptEntry] = physAddr | PG_PRESENT | PG_READ_WRITE;
    return TRUE;
}

boolean MmUnmapPage(PageTable* table, u64 virtAddr){
    u64 pml4Entry = GET_PML4_ENTRY(virtAddr);
    u64 pdptEntry = GET_PDPT_ENTRY(virtAddr);
    u64 pdEntry = GET_PD_ENTRY(virtAddr);
    u64 ptEntry = GET_PT_ENTRY(virtAddr);
    if(table->pml4[pml4Entry] == 0x0){
        return FALSE; // isn't mapped
    }
    uptr* pdpt = PARSE_ENTRY_VALUE(table->pml4[pml4Entry]);
    if(pdpt[pdptEntry] == 0x0){
        return FALSE; // isn't mapped
    }
    uptr* pd = PARSE_ENTRY_VALUE(pdpt[pdptEntry]);
    if(pd[pdEntry] == 0x0){
        return FALSE; // isn't mapped
    }
    uptr* pt = PARSE_ENTRY_VALUE(pd[pdEntry]);
    pt[ptEntry] = 0;
    MmInvalidatePage(virtAddr);
    return TRUE;
}


PageTable* MmReturnKernelPageTable(){
    return &kernPTable;
}

boolean MmInitVirtualMemoryManager(BootInfo* info){
    if(kernPTable.pml4 != NULL){
        MmFreeSinglePage(kernPTable.pml4);
    }
    kernPTable.pml4 = MmAllocateSinglePage();
    memset(kernPTable.pml4, 0, PAGE_SIZE);
    for(u64 i = 0; i < info->kernelSizeInPages; i++){
        boolean result = MmMapPage(&kernPTable, info->kernelLocPhys + (i * PAGE_SIZE), 
                                    info->kernelLocVirt + (i * PAGE_SIZE), PG_READ_WRITE);
        if(result == FALSE) return FALSE;
    }
    for(u64 i = 0; i < info->memMap->amountOfEntries; i++){
        u64 base = info->memMap->memEntries[i].base;
        u64 size = info->memMap->memEntries[i].size;
        for(u64 j = 0; j < size; j+=PAGE_SIZE){
            boolean result = MmMapPage(&kernPTable, base + j, base + j, PG_READ_WRITE);
            if(result == FALSE) return FALSE;
        }
    }
    for(u64 i = 0; i < KeGraphicsReturnData()->framebufferSize; i+=0x1000){
        MmMapPage(&kernPTable, KeGraphicsReturnData()->framebufferBase + i, 
                     KeGraphicsReturnData()->framebufferBase + i, PG_READ_WRITE);
    }
    #ifdef PAGEMAP_PMM
    PageMap* pageMap = MmReturnPageMap();
    u64 base = (u64)(pageMap);
    u64 size = MmReturnPageAmount();
    for(u64 i = 0; i < size; i++){
        boolean result = MmMapPage(&kernPTable, base + (i * PAGE_SIZE), 
                                base + (i * PAGE_SIZE), PG_READ_WRITE);
        if(result == FALSE) return FALSE;
    }
    #endif
    MmUpdateCr3((u64)kernPTable.pml4);
    return TRUE;
}