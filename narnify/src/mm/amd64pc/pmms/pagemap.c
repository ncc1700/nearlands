#include <nrstatus.h>
#include <mm/pmm.h>



/**
    Very slow physical memory manager

    I wrote this in like 30m, its also very unsafe 

    ill replace this with a bitmap allocator in the future but
    for now it works =)


*/

#ifdef PAGEMAP_PMM


static PageMap* pageMap = NULL;
static u64 pageMapAmount = 0;
static u64 pageUsed = 0;

#define INCREMENT_PAGE_USAGE() if(pageUsed < pageMapAmount) pageUsed++;
#define DECREMENT_PAGE_USAGE() if(pageUsed > 0) pageUsed--;

NearStatus MmInitPhysicalMemoryManager(BootMemoryMap* memMap){
    // we calculate the size of the pagemap
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;
        if(memMap->memEntries[i].base == 0x0){
            memMap->memEntries[i].base += PAGE_SIZE;
            memMap->memEntries[i].size -= PAGE_SIZE;
        }
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        for(u64 j = base; j < (base + size); j+=PAGE_SIZE){
            pageMapAmount++;
        }
    }
    if(pageMapAmount == 0){
        return STATUS_NO_VALID_MEMORY;
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
        if(size >= (pageMapAmount * sizeof(PageMap))){
            pageMap = (PageMap*)base;
            u64 pagemapSize = (pageMapAmount * sizeof(PageMap));
            u64 pmsizeAligned = (pagemapSize + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1); 
            memMap->memEntries[i].size -= (pmsizeAligned);
            memMap->memEntries[i].base += (pmsizeAligned);
            break;
        }   
    }
    if(pageMap == NULL){
        return STATUS_NOT_ENOUGH_MEMORY;
    }
    // we fill the pagemap
    u64 curPageMap = 0;
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;
        u64 base = (memMap->memEntries[i].base + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
        u64 size = (memMap->memEntries[i].size) & ~(PAGE_SIZE - 1);
        for(u64 j = base; j < (base + size); j+=PAGE_SIZE){
            pageMap[curPageMap].address = j;
            pageMap[curPageMap].isFree = TRUE;
            curPageMap++;
        }
    }
    return STATUS_SUCCESS;
}

void* MmAllocateSinglePage(){
    for(u64 i = 0; i < pageMapAmount; i++){
        if(pageMap[i].isFree == TRUE){
            pageMap[i].isFree = FALSE;
            INCREMENT_PAGE_USAGE();
            return (void*)pageMap[i].address;
        }
    }
    return NULL;
}

void* MmAllocateMultiplePages(u64 amount){
    u64 indexToAlloc = 0;
    boolean shouldAlloc = FALSE;
    for(u64 i = 0; i < pageMapAmount; i++){
        if((i + amount) >= pageMapAmount) break;
        if(pageMap[i].isFree == TRUE){
            u64 base = i;
            u64 prevAddress = 0;
            boolean good = TRUE;
            for(u64 j = 0; j < amount; j++){
                if(pageMap[base + j].isFree == FALSE){
                    good = FALSE;
                    break;
                }
                u64 curAddress = pageMap[base + j].address;
                if(prevAddress != 0){
                    if((curAddress - prevAddress) != PAGE_SIZE){
                        good = FALSE;
                        break;
                    }
                }
                prevAddress = curAddress;
            }
            if(good == TRUE){
                shouldAlloc = TRUE;
                indexToAlloc = i;
                break;
            }
        }
    }
    if(shouldAlloc == TRUE){
        for(u64 i = 0; i < amount; i++){
            pageMap[indexToAlloc + i].isFree = FALSE;
            INCREMENT_PAGE_USAGE();
        }
        return (void*)pageMap[indexToAlloc].address;
    }
    return NULL;
}

NearStatus MmFreeSinglePage(void* pageAddress){
    // VERY SLOW: TODO: MAKE IT BETTER
    for(u64 i = 0; i < pageMapAmount; i++){
        if(pageMap[i].address == (u64)pageAddress){
            pageMap[i].isFree = TRUE;
            DECREMENT_PAGE_USAGE();
            return STATUS_SUCCESS;
        }
    }
    return STATUS_NOT_FOUND;
}

NearStatus MmFreeMultiplePages(void* pageAddress, u64 amount){
    // VERY SLOW: MAKE IT BETTER
    for(u64 i = 0; i < (pageMapAmount - amount); i++){
        if(pageMap[i].address == (u64)pageAddress){
            for(u64 j = 0; j < amount; j++){
                pageMap[i + j].isFree = TRUE;
                DECREMENT_PAGE_USAGE();
            }
            return STATUS_SUCCESS;
        }
    }
    return STATUS_NOT_FOUND;
}

u64 MmReturnPageUsed(){
    return pageUsed;
}

u64 MmReturnPageAmount(){
    return pageMapAmount;
}

PageMap* MmReturnPageMap(){
    return pageMap;
}



#endif