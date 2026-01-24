#include "../includes/mm.h"
#include "../includes/pmm.h"
#include "../includes/vmm.h"
#include "../includes/heap.h"
#include "../../ke/term.h"







boolean MmInitSystem(BootInfo* info){
    boolean result = MmInitPhysicalMemoryManager(info->memMap);
    KeTermPrint(TERM_STATUS_INFO, QSTR("setting up the physical memory manager"));
    if(result == FALSE){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("Couldn't Setup Physical Memory Manager"));
        return FALSE;
    }
    KeTermPrint(TERM_STATUS_INFO, QSTR("initalized %d pages"), MmReturnPageAmount());
    KeTermPrint(TERM_STATUS_INFO, QSTR("testing the physical memory manager"));
    result = MmTestPhysicalMemoryManager();
    if(result == FALSE){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("Physical Memory Manager failed tests"));
        return FALSE;
    }
    KeTermPrint(TERM_STATUS_INFO, QSTR("setting up the virtual memory manager"));
    result = MmInitVirtualMemoryManager(info);
    if(result == FALSE){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("Couldn't Setup virtual Memory Manager"));
        return FALSE;
    }
    u64 heapSize = info->memMap->sizeOfEntireMemory / 23;
    u64 heapSizeInPages = (heapSize + (PAGE_SIZE - 1)) / PAGE_SIZE;
    KeTermPrint(TERM_STATUS_INFO, QSTR("setting up the kernel heap (%d pages)"), heapSizeInPages);
    result = MmInitHeapAllocator(heapSizeInPages);
    if(result == FALSE){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("Couldn't Setup Heap Allocator"));
        return FALSE;
    }
    return TRUE;
}