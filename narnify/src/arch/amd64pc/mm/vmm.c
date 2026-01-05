#include "vmm.h"
#include "pmm.h"
#include "../../../qol.h"
#include "../../../term.h"

#include "../../includes/serial.h"



__attribute__((aligned(4096))) static uptr pml4[512] = {0};


#define PG_PRESENT 0x1
#define PG_READ_WRITE 0x2
#define PG_PS 0x80

#define GET_PML4_ENTRY(x) ((x & ((u64)0x1ff << 39)) >> 39)
#define GET_PDPT_ENTRY(x) ((x & ((u64)0x1ff << 30)) >> 30)
#define GET_PD_ENTRY(x) ((x & ((u64)0x1ff << 21)) >> 21)
#define GET_PT_ENTRY(x) ((x & ((u64)0x1ff << 12)) >> 12)

#define PARSE_ENTRY_VALUE(x) (uptr*)(x & ~0xFFF)

static inline boolean GetNextLevel(uptr* pml, u64 entry, u64 flags){
    //if(entry >= 512) return FALSE;
    uptr* address = (uptr*)MmAllocateSinglePage();
    
    //TermPrint(TERM_STATUS_INFO, "address: 0x%x", address);
    if(address == NULL){
        return FALSE;
    }
    if((uptr)address == 0x0) return FALSE;
    memset(address, 0, PAGE_SIZE);
    pml[entry] = (uptr)address | PG_PRESENT | PG_READ_WRITE;
    
    return TRUE;
}


boolean MmMapPage(u64 physAddr, u64 virtAddr, u64 flags){
    u64 pml4Entry = GET_PML4_ENTRY(virtAddr);
    u64 pdptEntry = GET_PDPT_ENTRY(virtAddr);
    u64 pdEntry = GET_PD_ENTRY(virtAddr);
    u64 ptEntry = GET_PT_ENTRY(virtAddr);
    if(pml4[pml4Entry] == 0x0){
        boolean result = GetNextLevel(pml4, pml4Entry, 0);
        if(result == FALSE){
            return FALSE;
        }
    }

    uptr* pdpt = PARSE_ENTRY_VALUE(pml4[pml4Entry]);
    // TermPrint(TERM_STATUS_INFO, "pdpt: 0x%x", pdpt);
    // TermPrint(TERM_STATUS_INFO, "pdpt has 0x%x (%d) inside", pdpt[pdptEntry]);
    if(pdpt[pdptEntry] == 0x0){
        boolean result = GetNextLevel(pdpt, pdptEntry, 0);
        if(result == FALSE){
            return FALSE;
        }
    }

    uptr* pd = PARSE_ENTRY_VALUE(pdpt[pdptEntry]);
    //TermPrint(TERM_STATUS_INFO, "pd: 0x%x", pd); // this becomes 0
    if(pd[pdEntry] == 0x0){
        boolean result = GetNextLevel(pd, pdEntry, 0);
        if(result == FALSE){
            return FALSE;
        }
    }
    uptr* pt = PARSE_ENTRY_VALUE(pd[pdEntry]);
    pt[ptEntry] = physAddr | PG_PRESENT | PG_READ_WRITE;
    //ArPrintToSerial("j\n");

    return TRUE;
}

#define HHDM_OFFSET 0xFFFFFF8000000000ULL

boolean MmInitVirtualMemoryManager(BootInfo* info){
    TermPrint(TERM_STATUS_INFO, "kernphys = 0x%x, kernvirt = 0x%x", info->kernelLocPhys, info->kernelLocVirt);
    memset(pml4, 0, PAGE_SIZE);
    for(u64 i = 0; i < 0x40000000; i+=PAGE_SIZE){
        boolean result = MmMapPage(info->kernelLocPhys + i, 
                                    HHDM_OFFSET + i, 0);
        if(result == FALSE) return FALSE;
    }
    // for(u64 i = 0; i < info->memMap->amountOfEntries; i++){
    //     u64 base = info->memMap->memEntries[i].base;
    //     u64 size = info->memMap->memEntries[i].size;
    //     for(u64 j = base; j < (base + size); j+=0x1000){
    //         MmMapPage(j, j, 0);
    //     }
    // }
    u64 size = info->memMap->sizeOfEntireMemory + 0x100000000;
    for(u64 i = 0x0; i < size; i+=0x1000){
        boolean result = MmMapPage(i, i, 0);
        if(result == FALSE) return FALSE;
    }
    u64 offset = info->kernelLocVirt - (u64)pml4;
    u64 pml4PhysLoc = info->kernelLocPhys + offset;
    TermPrint(TERM_STATUS_INFO, "Updating Cr3");
    //MmUpdateCr3((u64)pml4PhysLoc);
    //ArPrintToSerial("j\n");
    return TRUE;
}