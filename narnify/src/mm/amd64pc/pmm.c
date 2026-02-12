#include "ke/term.h"
#include <mm/pmm.h>
#include <qol/qmem.h>


/*
    the actual physical memory manager/allocators are
    located in the pmm folder, with each impl being
    a single file


    this is just a place to test such PMMs
*/






boolean MmTestPhysicalMemoryManager(){
    void* o = MmAllocateSinglePage();
    if(o == NULL) return FALSE;
    //MmFreeSinglePage(o);
    void* h = MmAllocateMultiplePages(11);
    void* p = MmAllocateMultiplePages(2);
    if(h == NULL || p == NULL) return FALSE;
    u64 hAddress = (u64)h;
    MmFreeMultiplePages(h, 10);
    void* c = MmAllocateMultiplePages(10);
    if(c == NULL) return FALSE;
    if((u64)c != hAddress){
        MmFreeMultiplePages(p, 2);
        MmFreeMultiplePages(c, 10);
        return FALSE;
    }
    u64 pAddress = (u64)p;
    MmFreeMultiplePages(p, 2);
    void* l = MmAllocateMultiplePages(2);
    if(l == NULL) return FALSE;
    if((u64)l != pAddress){
        MmFreeMultiplePages(c, 10);
        MmFreeMultiplePages(l, 11);
        return FALSE;
    }
    MmFreeMultiplePages(c, 10);
    MmFreeMultiplePages(l, 11);    
    return TRUE;
}


void* MmReallocatePages(void* memory, u64 prevSizeInPages, u64 newSizeInPages){
    void* newMemory = MmAllocateMultiplePages(newSizeInPages);
    if(newMemory == NULL) return NULL;
    memset(newMemory, 0, newSizeInPages * PAGE_SIZE);
    if(memory != NULL){
        memcpy(newMemory, memory, prevSizeInPages * PAGE_SIZE);
        MmFreeMultiplePages(memory, prevSizeInPages);
    }
    return newMemory;
}