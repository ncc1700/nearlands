#include "nrstatus.h"
#include <mm/vmm.h>
#include <mm/pmm.h>
#include <ke/graphics.h>
#include <qol/qmem.h>


static PageTable kernPTable = {NULL};




#ifdef PAGEMAP_PMM
extern PageMap* MmReturnPageMap();
#endif

static inline NearStatus GetNextLevel(uptr* pml, u64 entry, u64 flags){
    if(entry >= 512) return STATUS_INVALID_PML_ENTRY;
    uptr* address = (uptr*)MmAllocateSinglePage();
    
    if(address == NULL){
        return STATUS_OUT_OF_MEMORY;
    }
    if((uptr)address == 0x0) return STATUS_OUT_OF_MEMORY;
    memset(address, 0, PAGE_SIZE);
    pml[entry] = (uptr)address | PG_PRESENT | PG_READ_WRITE;
    
    return STATUS_SUCCESS;
}


NearStatus MmMapPage(PageTable* table, u64 physAddr, u64 virtAddr, u64 flags){
    u64 pml4Entry = GET_PML4_ENTRY(virtAddr);
    u64 pdptEntry = GET_PDPT_ENTRY(virtAddr);
    u64 pdEntry = GET_PD_ENTRY(virtAddr);
    u64 ptEntry = GET_PT_ENTRY(virtAddr);
    if(table->pml4[pml4Entry] == 0x0){
        NearStatus result = GetNextLevel(table->pml4, pml4Entry, flags | PG_READ_WRITE);
        if(!NR_SUCCESS(result)){
            return result;
        }
    }
    uptr* pdpt = PARSE_ENTRY_VALUE(table->pml4[pml4Entry]);
    if(pdpt[pdptEntry] == 0x0){
        NearStatus result = GetNextLevel(pdpt, pdptEntry, flags | PG_READ_WRITE);
        if(!NR_SUCCESS(result)){
            return result;
        }
    }
    uptr* pd = PARSE_ENTRY_VALUE(pdpt[pdptEntry]);
    if(pd[pdEntry] == 0x0){
        NearStatus result = GetNextLevel(pd, pdEntry, flags);
        if(!NR_SUCCESS(result)){
            return result;
        }
    }
    uptr* pt = PARSE_ENTRY_VALUE(pd[pdEntry]);
    pt[ptEntry] = physAddr | PG_PRESENT | PG_READ_WRITE;
    return STATUS_SUCCESS;
}

NearStatus MmUnmapPage(PageTable* table, u64 virtAddr){
    u64 pml4Entry = GET_PML4_ENTRY(virtAddr);
    u64 pdptEntry = GET_PDPT_ENTRY(virtAddr);
    u64 pdEntry = GET_PD_ENTRY(virtAddr);
    u64 ptEntry = GET_PT_ENTRY(virtAddr);
    if(table->pml4[pml4Entry] == 0x0){
        return STATUS_NOT_MAPPED; 
    }
    uptr* pdpt = PARSE_ENTRY_VALUE(table->pml4[pml4Entry]);
    if(pdpt[pdptEntry] == 0x0){
        return STATUS_NOT_MAPPED; 
    }
    uptr* pd = PARSE_ENTRY_VALUE(pdpt[pdptEntry]);
    if(pd[pdEntry] == 0x0){
        return STATUS_NOT_MAPPED; 
    }
    uptr* pt = PARSE_ENTRY_VALUE(pd[pdEntry]);
    pt[ptEntry] = 0;
    MmInvalidatePage(virtAddr);
    return STATUS_SUCCESS;
}

boolean MmCheckIfPageIsMapped(PageTable* table, u64 virtAddr){
    u64 pml4Entry = GET_PML4_ENTRY(virtAddr);
    u64 pdptEntry = GET_PDPT_ENTRY(virtAddr);
    u64 pdEntry = GET_PD_ENTRY(virtAddr);
    u64 ptEntry = GET_PT_ENTRY(virtAddr);
    if(table->pml4[pml4Entry] == 0x0){
        return FALSE; 
    }
    uptr* pdpt = PARSE_ENTRY_VALUE(table->pml4[pml4Entry]);
    if(pdpt[pdptEntry] == 0x0){
        return FALSE; 
    }
    uptr* pd = PARSE_ENTRY_VALUE(pdpt[pdptEntry]);
    if(pd[pdEntry] == 0x0){
        return FALSE; 
    }
    uptr* pt = PARSE_ENTRY_VALUE(pd[pdEntry]);
    if(pt[ptEntry] == 0x0){
        return FALSE;
    }
    else return TRUE;
}


PageTable* MmReturnKernelPageTable(){
    return &kernPTable;
}

NearStatus MmInitVirtualMemoryManager(BootInfo* info){
    if(kernPTable.pml4 != NULL){
        MmFreeSinglePage(kernPTable.pml4);
    }
    kernPTable.pml4 = MmAllocateSinglePage();
    memset(kernPTable.pml4, 0, PAGE_SIZE);
    for(u64 i = 0; i < info->kernelSizeInPages; i++){
        NearStatus result = MmMapPage(&kernPTable, info->kernelLocPhys + (i * PAGE_SIZE), 
                                    info->kernelLocVirt + (i * PAGE_SIZE), PG_READ_WRITE);
        if(!NR_SUCCESS(result)) return result;
    }
    for(u64 i = 0; i < info->memMap->amountOfEntries; i++){
        u64 base = info->memMap->memEntries[i].base;
        u64 size = info->memMap->memEntries[i].size;
        for(u64 j = 0; j < size; j+=PAGE_SIZE){
            NearStatus result = MmMapPage(&kernPTable, base + j, base + j, PG_READ_WRITE);
            if(!NR_SUCCESS(result)) return result;
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
        NearStatus result = MmMapPage(&kernPTable, base + (i * PAGE_SIZE), 
                                base + (i * PAGE_SIZE), PG_READ_WRITE);
        if(!NR_SUCCESS(result)) return result;
    }
    #endif
    MmUpdateCr3((u64)kernPTable.pml4);
    return STATUS_SUCCESS;
}