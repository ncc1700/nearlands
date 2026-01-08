#include "../../includes/heap.h"
#include "../../includes/pmm.h"
#include "../../../term.h"


/**
    Very bad heap but works i think

*/

#define HEAP_BLOCK_SIZE 32

typedef struct _FreeList {
    u64 address;
    struct _FreeList* next;
    struct _FreeList* prev;
    u16 size;
    boolean isFree;
} __attribute__((packed)) FreeList;

static u64* heapMem = NULL;
static u64 curIndex = 0;
static u64 heapSize = 0;
static FreeList* freeList = NULL;
static FreeList* prevIndex = NULL;
static FreeList* initial = NULL;


boolean MmInitHeapAllocator(u64 amountInPages){
    TermPrint(TERM_STATUS_INFO, "h");
    heapMem = MmAllocateMultiplePages(amountInPages);
    TermPrint(TERM_STATUS_INFO, "h");
    if(heapMem == NULL) return FALSE;    
    heapSize = amountInPages * PAGE_SIZE;
    return TRUE;
}


void* MmAllocateFromHeap(u16 size){
    u64 trueSize = 0;
    if(size < sizeof(FreeList)) trueSize = sizeof(FreeList) + 1;
    else trueSize = size;
    if(trueSize >= 0x1000){
        return NULL;
    }
    FreeList* list = initial;
    while(list != NULL){
        if(list->isFree == TRUE){
            if(list->size == trueSize){
                list->next->prev = list->prev;
                list->prev->next = list->next;
                list->isFree = FALSE;
                return (void*)list->address;
            }
            if(list->size > trueSize){
                u64 cut = list->size - trueSize;
                list->size = cut;
                u64 address = list->address;
                list->address = (u64)((u64)list->address + trueSize);
                return (void*)(address);
            }
        }
        list = list->next;
    } 
    if(heapSize <= (curIndex + trueSize)){
        return NULL; // kernel heap ran out of memory;
    }
    u64 prevIndex = curIndex;
    curIndex += trueSize;
    return (void*)((u64)heapMem + curIndex);
}


boolean MmFreeFromHeap(void* address, u16 size){
    u64 trueSize = 0;
    if(size < sizeof(FreeList)) trueSize = sizeof(FreeList) + 1;
    else trueSize = size;
    if(trueSize >= 0x1000){
        return FALSE;
    }
    FreeList* cur = (FreeList*)address;
    cur->address = (u64)address;
    cur->size = trueSize;
    cur->isFree = TRUE;
    if(initial == NULL){
        freeList = cur;
        freeList->next = NULL;
        freeList->prev = NULL;
        initial = freeList;
        prevIndex = freeList;
        freeList = freeList->next;
    } else {
        freeList = cur;
        freeList->prev = prevIndex;
        prevIndex = freeList;
        freeList->next = NULL;
    }
    return TRUE;
}


boolean MmTestHeapAllocator(){
    // REIMPLEMENT
    return FALSE;
}