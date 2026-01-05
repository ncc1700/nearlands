#include "../../../includes/pmm.h"






#ifdef BITMAP_PMM




boolean MmInitPhysicalMemoryManager(BootMemoryMap* memMap){
    // TODO: implement
    return FALSE;
}

void* MmAllocateSinglePage(){
    // TODO: implement
    return NULL;
}

void* MmAllocateMultiplePages(u64 amount){
    // TODO: implement
    return NULL;
}

boolean MmFreeSinglePage(void* pageAddress){
    // TODO: implement
    return NULL;
}

boolean MmFreeMultiplePages(void* pageAddress, u64 amount){
    // TODO: implement
    return NULL;
}


#endif