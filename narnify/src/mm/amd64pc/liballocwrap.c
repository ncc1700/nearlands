#include "ke/spinlock.h"
#include "mm/pmm.h"
#include "nrstatus.h"
#include <mm/alloc.h>
#include <liballoc/liballoc.h>

/*

    this is here so i can use liballoc for testing if an
    issue is within my own homemade allocator or if its within
    something else


*/

#ifdef USE_LIBALLOC

static SpinLock lock = {0};

NearStatus MmInitGeneralAllocator(){
    lock = KeCreateSpinLock();
    return STATUS_SUCCESS;
}

void* MmAllocateGeneralMemory(u64 allocSize){
    return malloc(allocSize);
}
void MmSetAllowFrees(boolean value){
    return; // not used
}
NearStatus MmFreeGeneralMemory(void* address){
    free(address);
    return STATUS_SUCCESS;
}

int liballoc_lock() {
	KeAcquireSpinLock(&lock);
	return 0;
}

int liballoc_unlock() {
	KeReleaseSpinLock(&lock);
	return 0;
}

void* liballoc_alloc(int pages) {
	void* p = MmAllocateMultiplePages(pages);
	return p;
}

int liballoc_free(void* ptr, int pages){
	NearStatus status = MmFreeMultiplePages(ptr, pages);
    if(!NR_SUCCESS(status)){
        return -1;
    } else return 0;
}




#endif