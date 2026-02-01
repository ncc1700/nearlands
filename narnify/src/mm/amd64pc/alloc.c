#include "ke/term.h"
#include "mm/pmm.h"
#include "types.h"
#include <mm/alloc.h>
#include <qol/qmem.h>

typedef struct _FreeList {
    u64 address;
    struct _FreeList* next;
    struct _FreeList* prev;
    struct _FreeList* header;
    u16 size;
    u16 memSize;
    boolean isFree;
} __attribute__((packed)) FreeList;


static void* allocBase = 0;
static void* allocCur = 0;
static void* allocLimit = 0;

static FreeList* head = NULL;
static FreeList* tail = NULL;
static const boolean allowFrees = TRUE;


static inline void AddToFreeList(FreeList* list){
    list->next = NULL;
    list->prev = tail;
    if(tail != NULL) tail->next = list;
    tail = list;
    if(head == NULL){
        head = list;
    } 
}

static inline void RemoveFromFreeList(FreeList* list){
    if(list == head){
        head = list->next;
    }
    if(list == tail){
        tail = list->prev;
    }
    FreeList* prev = list->prev;
    FreeList* next = list->next;
    
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    list->prev = NULL;
    list->next = NULL;
}

boolean MmInitGeneralAllocator(){
    allocBase = MmAllocateSinglePage();
    if(allocBase == NULL) return FALSE;
    allocCur = allocBase;
    allocLimit = allocBase + PAGE_SIZE;
    return TRUE;
}


void* MmAllocateGeneralMemory(u64 allocSize){
    if(allocSize >= PAGE_SIZE){
        KeTermPrint(TERM_STATUS_INFO, QSTR("Someone tried to allocate memory above 4KB!"));
        return NULL;
    }
    u64 size = allocSize + sizeof(FreeList);
    if(allowFrees == TRUE){
        FreeList* cur = head;
        while(cur != NULL){
            if(cur->isFree == TRUE){
                if(cur->size < size){
                    cur = cur->next;
                    continue;
                } else if(cur->size == size){
                    RemoveFromFreeList(cur);
                    cur->isFree = FALSE;
                    return (void*)cur->address;
                } else if(cur->size > size){
                    u64 allocAddr = cur->address;
                    cur->size -= size;
                    cur->address += size;
                    FreeList* header = (FreeList*)allocAddr;
                    memset(header, 0, sizeof(FreeList));
                    allocAddr += sizeof(FreeList);
                    header->isFree = TRUE;
                    header->address = (u64)allocAddr;
                    header->size = size;
                    header->memSize = allocSize;
                    header->header = header;
                    return (void*)allocAddr;
                }
            }
            cur = cur->next;
        }
    }
    if(((u64)allocCur + size) >= (u64)allocLimit){
        KeTermPrint(TERM_STATUS_INFO, QSTR("expanding heap outreach"));
        u64 amountUntilLimit = (u64)allocLimit - (u64)allocCur;
        if(amountUntilLimit >= sizeof(FreeList)){
            FreeList* header = allocCur;
            memset(header, 0, sizeof(FreeList));
            u64 allocCurAddr = (u64)allocCur;
            allocCurAddr += sizeof(FreeList);
            allocCur = (void*)allocCurAddr;
            header->isFree = FALSE;
            header->address = (u64)allocCur;
            header->size = amountUntilLimit;
            header->memSize = amountUntilLimit - sizeof(FreeList);
            header->header = header;
            MmFreeGeneralMemory(allocCur);
            // we now reinit the allocator
        }
        MmInitGeneralAllocator();
    }
    FreeList* header = allocCur;
    memset(header, 0, sizeof(FreeList));
    u64 allocCurAddr = (u64)allocCur;
    allocCurAddr += sizeof(FreeList);
    allocCur = (void*)allocCurAddr;
    header->isFree = FALSE;
    header->address = (u64)allocCur;
    header->size = size;
    header->memSize = allocSize;
    header->header = header;
    void* prev = allocCur;
    allocCurAddr = (u64)allocCur;
    allocCurAddr += allocSize;
    allocCur = (void*)allocCurAddr;
    return prev;
}

boolean MmFreeGeneralMemory(void* address){
    FreeList* header = address - sizeof(FreeList);
    header->isFree = TRUE;
    AddToFreeList(header);
    return TRUE;
}