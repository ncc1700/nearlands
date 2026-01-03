#include "../includes/mem.h"
#include "../../qol.h"



__attribute__((aligned(4096))) static uptr pml4[512] = {0};



#define PG_PRESENT 0x1
#define PG_READ_WRITE 0x2
#define PG_PS 0x80

#define GET_PML4_ENTRY(x) ((x & ((u64)0x1ff << 39)) >> 39)
#define GET_PDPT_ENTRY(x) ((x & ((u64)0x1ff << 30)) >> 30)
#define GET_PD_ENTRY(x) ((x & ((u64)0x1ff << 21)) >> 21)
#define GET_PT_ENTRY(x) ((x & ((u64)0x1ff << 12)) >> 12)

#define PARSE_ENTRY_VALUE(x) (uptr*)(x & ~0xFFF)

static inline boolean GetNextLevel(uptr* address){
    uptr addr = 0x0;
    EFI_STATUS status = QolReturnSystemTable()->BootServices->AllocatePages(AllocateAnyPages, 
                                                                        EfiLoaderCode, 1, &addr);
    if(status != EFI_SUCCESS){
        // TODO: panic;
        return FALSE;
    }
    uptr* ptr = (uptr*)addr;
    //QolReturnSystemTable()->BootServices->SetMem(ptr, 0x1000, 0);
    memset(ptr, 0, 0x1000);
    *address = addr;
    return TRUE;
}

boolean LdrMmMapSinglePage(u64 address, u64 virtAddress){
    u64 pml4Entry = GET_PML4_ENTRY(virtAddress);
    u64 pdptEntry = GET_PDPT_ENTRY(virtAddress);
    u64 pdEntry = GET_PD_ENTRY(virtAddress);
    u64 ptEntry = GET_PT_ENTRY(virtAddress);

    if(pml4[pml4Entry] == 0){
        uptr pdptAddr;
        boolean result = GetNextLevel(&pdptAddr);
        if(result == FALSE){
            return FALSE;
        }
        pml4[pml4Entry] = pdptAddr | PG_PRESENT | PG_READ_WRITE;
    } 
    uptr* pdpt = PARSE_ENTRY_VALUE(pml4[pml4Entry]);
    if(pdpt[pdptEntry] == 0){
        uptr pdAddr;
        boolean result = GetNextLevel(&pdAddr);
        if(result == FALSE){
            return FALSE;
        }
        pdpt[pdptEntry] = pdAddr | PG_PRESENT | PG_READ_WRITE;
    }
    uptr* pd = PARSE_ENTRY_VALUE(pdpt[pdptEntry]);
    if(pd[pdEntry] == 0){
        uptr ptAddr;
        boolean result = GetNextLevel(&ptAddr);
        if(result == FALSE){
            return FALSE;
        }
        pd[pdEntry] = ptAddr | PG_PRESENT | PG_READ_WRITE;
    }

    u64* pt = PARSE_ENTRY_VALUE(pd[pdEntry]);
    pt[ptEntry] = address | PG_PRESENT | PG_READ_WRITE;
    return TRUE;
}


// HORRIBLE: PLEASE DO NOT DO THIS BRO
boolean LdrMmInitPaging(){
    // TODO: create a way to free an allocated memory map
    MemoryMap* memMap = LdrMmRetrieveCurrentMemoryMap();
    u64 size = memMap->sizeOfEntireMemory + 0x100000000;
    for(u64 i = 0x0; i < size; i+=0x1000){
        boolean result = LdrMmMapSinglePage(i, i);
        if(result == FALSE) return FALSE;
    }
    LdrMmUpdateCr3((u64)pml4);
    return TRUE;
}

// maps 1gb of memory starting from kernel address
boolean LdrMmMapHigherHalfMemoryForKernel(u64 address){
    u64 addressPlus1g = address + 0x40000000;
    for(u64 i = 0; i < 0x40000000; i+=0x1000){
        boolean result = LdrMmMapSinglePage(address + i, HHDM_OFFSET + i);
        if(result == FALSE) return FALSE;
    }
    LdrMmUpdateCr3((u64)pml4);
    return TRUE;
}