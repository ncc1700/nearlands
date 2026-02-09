#include "mm/alloc.h"
#include "nrstatus.h"
#include <mm/mm.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <ke/term.h>
#include <qol/qmem.h>





NearStatus MmInitSystem(BootInfo* info){
    u64 memoryAmount = info->memMap->sizeOfEntireMemory;
    u64 kernelHeapDivisor = 1;
    if(memoryAmount < MB(1)){
        KeTermPrint(TERM_STATUS_INFO, 
            QSTR("System has %d KB of RAM"), TO_KB(memoryAmount));
    } else if(memoryAmount < GB(1)){
        KeTermPrint(TERM_STATUS_INFO, 
            QSTR("System has %d MB of RAM"), TO_MB(memoryAmount));
    } else if(memoryAmount < TB(1)){
        KeTermPrint(TERM_STATUS_INFO, 
            QSTR("System has %d GB of RAM"), TO_GB(memoryAmount));
    } else {
        KeTermPrint(TERM_STATUS_INFO, 
            QSTR("System has %d TB of RAM"), TO_TB(memoryAmount));
    }
    NearStatus status = MmInitPhysicalMemoryManager(info->memMap);
    KeTermPrint(TERM_STATUS_INFO, QSTR("setting up the physical memory manager"));
    if(!NR_SUCCESS(status)){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("Couldn't Setup Physical Memory Manager"));
        return status;
    }
    KeTermPrint(TERM_STATUS_INFO, QSTR("initialized %d pages"), MmReturnPageAmount());
    KeTermPrint(TERM_STATUS_INFO, QSTR("testing the physical memory manager"));
    boolean testRes = MmTestPhysicalMemoryManager();
    if(testRes == FALSE){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("Physical Memory Manager failed tests"));
        return STATUS_PMM_FAIL;
    }
    KeTermPrint(TERM_STATUS_INFO, QSTR("setting up the virtual memory manager"));
    status = MmInitVirtualMemoryManager(info);
    if(!NR_SUCCESS(status)){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("Couldn't Setup the virtual Memory Manager"));
        return status;
    }
    KeTermPrint(TERM_STATUS_INFO, QSTR("setting up the general memory allocator"));
    status = MmInitGeneralAllocator();
    if(!NR_SUCCESS(status)){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("Couldn't Setup the general memory allocator"));
        return STATUS_GALLOC_FAIL;
    }
    return STATUS_SUCCESS;
}