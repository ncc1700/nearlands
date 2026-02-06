#include <nrstatus.h>
#include <mm/pmm.h>



/**
    Very slow physical memory manager

    I wrote this in like 30m, its also very unsafe 

    ill replace this with a bitmap allocator in the future but
    for now it works =)


*/

#ifdef LISTMAP_PMM

// u64 pp[] = {
//     1, 1, 1, 1, 0, 0, 1,
//     1, 1, 1, 1, 1, 1, 1,
// }


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
    // we now initialize all 
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;

    }
    return STATUS_SUCCESS;
}

void* MmAllocateSinglePage(){
    // UNIMPLEMENTED
    return NULL;
}

void* MmAllocateMultiplePages(u64 amount){
    // UNIMPLEMENTED
    return NULL;
}

NearStatus MmFreeSinglePage(void* pageAddress){
    // UNIMPLEMENTED
    return STATUS_NOT_FOUND;
}

NearStatus MmFreeMultiplePages(void* pageAddress, u64 amount){
    // UNIMPLEMENTED
    return STATUS_NOT_FOUND;
}

u64 MmReturnPageUsed(){
    return pageUsed;
}

u64 MmReturnPageAmount(){
    return pageMapAmount;
}




#endif