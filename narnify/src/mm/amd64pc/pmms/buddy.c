#include <nrstatus.h>
#include <mm/pmm.h>



/**
    Very slow physical memory manager

    I wrote this in like 30m, its also very unsafe 

    ill replace this with a bitmap allocator in the future but
    for now it works =)


*/

#ifdef BUDDY_PMM

// u64 pp[] = {
//     1, 1, 1, 1, 0, 0, 1,
//     1, 1, 1, 1, 1, 1, 1,
// }

typedef struct _Blocks {
    u64 addrStart;
    u64 addrEnd;
    u64 blockSplit;
} Blocks;

static u64 pageMapAmount = 0;
static u64 pageUsed = 0;
static u64 largestBlockPossible = 0;

static Blocks blocks[10];

#define BLOCK_SPLIT_HIGHEST

#define INCREMENT_PAGE_USAGE() if(pageUsed < pageMapAmount) pageUsed++;
#define DECREMENT_PAGE_USAGE() if(pageUsed > 0) pageUsed--;

NearStatus MmInitPhysicalMemoryManager(BootMemoryMap* memMap){
    
    
    return STATUS_NOT_IMPLEMENTED;
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
    return STATUS_NOT_IMPLEMENTED;
}

NearStatus MmFreeMultiplePages(void* pageAddress, u64 amount){
    // UNIMPLEMENTED
    return STATUS_NOT_IMPLEMENTED;
}

u64 MmReturnPageUsed(){
    return pageUsed;
}

u64 MmReturnPageAmount(){
    return pageMapAmount;
}




#endif