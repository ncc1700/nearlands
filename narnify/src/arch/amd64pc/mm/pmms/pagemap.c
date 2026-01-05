#include "../../../includes/pmm.h"
#include "../../../includes/serial.h"
#include "../../../../term.h"



/**
    Very slow physical memory manager

    I wrote this in like 30m, its also very unsafe 

    ill replace this with a bitmap allocator in the future but
    for now it works =)


*/

#ifdef PAGEMAP_PMM

typedef struct _PageMap {
    u64 address;
    boolean isFree:1;
}  __attribute__((packed)) PageMap;


static PageMap* pageMap = NULL;
static u64 pageMapAmount = 0;
static u64* freeList = NULL;


boolean MmInitPhysicalMemoryManager(BootMemoryMap* memMap){
    // we calculate the size of the pagemap
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;
        if(memMap->memEntries[i].base == 0x0){
            memMap->memEntries[i].base += PAGE_SIZE;
            memMap->memEntries[i].size -= PAGE_SIZE;
        }
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        if(base == 0x0) continue;
        for(u64 j = base; j < (base + size); j+=PAGE_SIZE){
            pageMapAmount++;
        }
    }
    if(pageMapAmount == 0){
        ArPrintToSerial("Couldn't calculate page map amount!\n");
        return FALSE;
    }
    
    // we find a place to put the pagemap in
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;
        if(memMap->memEntries[i].base == 0x0){
            memMap->memEntries[i].base += PAGE_SIZE;
            memMap->memEntries[i].size -= PAGE_SIZE;
        }
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        TermPrint(TERM_STATUS_INFO, "page map size is %d, size is %d\n", pageMapAmount, size);
        if(size >= (pageMapAmount * sizeof(PageMap))){
            TermPrint(TERM_STATUS_INFO, "base for pagemap is 0x%x\n", base);
            pageMap = (PageMap*)base;
            memMap->memEntries[i].size -= (pageMapAmount * sizeof(PageMap));
            memMap->memEntries[i].base += (pageMapAmount * sizeof(PageMap));
            break;
        }   
    }
    if(pageMap == NULL){
        ArPrintToSerial("Couldn't find space for page map!\n");
        return FALSE;
    }
    // we fill the pagemap
    u64 curPageMap = 0;
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        for(u64 j = base; j < (base + size); j+=PAGE_SIZE){
            pageMap[curPageMap].address = (j + 0xFFF) & ~0xFFF;
            pageMap[curPageMap].isFree = TRUE;
            curPageMap++;
        }
    }
    return TRUE;
}

void* MmAllocateSinglePage(){
    for(u64 i = 0; i < pageMapAmount; i++){
        if(pageMap[i].isFree == TRUE){
            pageMap[i].isFree = FALSE;
            return (void*)pageMap[i].address;
        }
    }
    return NULL;
}

void* MmAllocateMultiplePages(u64 amount){
    u64 cur = 0;
    while(cur + amount <= pageMapAmount){
        u64 prevAddress = pageMap[cur].address;
        boolean pass = TRUE;
        for(u64 i = 1; i < amount; i++){
            u64 curAddress = pageMap[cur + i].address;
            if((curAddress - prevAddress) != PAGE_SIZE || pageMap[cur + i].isFree == FALSE){
                pass = FALSE;
                break;
            }
            prevAddress = curAddress;
        }
        if(pass == TRUE){
            for(u64 i = 0; i < amount; i++){
                pageMap[cur + i].isFree = FALSE;
            }
            return (void*)pageMap[cur].address;
        } else cur++;
    }
    return NULL;
}

boolean MmFreeSinglePage(void* pageAddress){
    // VERY SLOW: TODO: MAKE IT BETTER
    for(u64 i = 0; i < pageMapAmount; i++){
        if(pageMap[i].address == (u64)pageAddress){
            pageMap[i].isFree = TRUE;
        }
    }
    return TRUE;
}

boolean MmFreeMultiplePages(void* pageAddress, u64 amount){
    // VERY SLOW AND UNSAFE: MAKE IT BETTER
    for(u64 i = 0; i < pageMapAmount; i++){
        if(pageMap[i].address == (u64)pageAddress){
            for(u64 j = 0; j < amount; j++){
                pageMap[i + j].isFree = TRUE;
            }
        }
    }
    return TRUE;
}


#endif