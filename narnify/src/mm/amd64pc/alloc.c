#include "ke/spinlock.h"
#include "ke/term.h"
#include "mm/pmm.h"
#include "types.h"
#include <mm/alloc.h>
#include <qol/qmem.h>


/**

    Bit better heap allocator, then the old one


    still scuffed but it..... works

*/

static SpinLock allocSpinLock = {0};

#define FREELIST_HEADER_MAGIC 0x6767

typedef struct _FreeList {
    u64 address;
    struct _FreeList* next;
    struct _FreeList* prev;
    struct _FreeList* header;
    u16 size;
    u16 memSize;
    u16 magic;
    boolean isFree;
} __attribute__((packed)) FreeList;


static void* allocBase = 0;
static void* allocCur = 0;
static void* allocLimit = 0;

static FreeList* head = NULL;
static FreeList* tail = NULL;
static boolean allowFrees = TRUE;


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

static inline boolean FreeMemory(void* address){
    if(address == NULL){
        return FALSE;
    }
    if(allowFrees == FALSE){
        return FALSE;
    }
    FreeList* header = address - sizeof(FreeList);

    if(header == NULL){
        return FALSE;
    }

    if(header->magic != FREELIST_HEADER_MAGIC){
        return FALSE;
    }

    header->isFree = TRUE;

    AddToFreeList(header);
    return TRUE;
}



static boolean ReinitGeneralAllocator(u64 minSize){
    u64 minSizeInPages = (minSize + (PAGE_SIZE - 1)) / PAGE_SIZE;
    allocBase = MmAllocateMultiplePages(minSizeInPages + PAGE_SIZE);
    if(allocBase == NULL) return FALSE;
    allocCur = allocBase;
    allocLimit = allocBase + PAGE_SIZE;
    return TRUE;
}

static inline void InitFreeList(FreeList* header, boolean isFree, 
                u64 totalSize, u64 memSize, u64 address)
{
    header->magic = FREELIST_HEADER_MAGIC;
    header->isFree = isFree;
    header->address = (u64)address;
    header->size = totalSize;
    header->memSize = memSize;
    header->header = header;
}

boolean MmInitGeneralAllocator(){
    allocSpinLock = KeCreateSpinLock();
    allocBase = MmAllocateSinglePage();
    if(allocBase == NULL) return FALSE;
    allocCur = allocBase;
    allocLimit = allocBase + PAGE_SIZE;
    return TRUE;
}

void* MmAllocateGeneralMemory(u64 allocSize){
    KeAcquireSpinLock(&allocSpinLock);
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
                    KeReleaseSpinLock(&allocSpinLock);
                    return (void*)cur->address;
                } else if(cur->size > size){
                    u64 allocAddr = cur->address;
                    cur->size -= size;
                    cur->address += size;
                    FreeList* header = (FreeList*)allocAddr;
                    memset(header, 0, sizeof(FreeList));
                    allocAddr += sizeof(FreeList);
                    InitFreeList(header, FALSE, size, 
                        allocSize, 
                        (u64)allocAddr);
                    KeReleaseSpinLock(&allocSpinLock);
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
            InitFreeList(header, TRUE, amountUntilLimit, 
                        amountUntilLimit - sizeof(FreeList), 
                        (u64)allocCur);
            FreeMemory(allocCur);
        }
        // we now reinit the allocator
        ReinitGeneralAllocator(size);
    }
    FreeList* header = allocCur;
    memset(header, 0, sizeof(FreeList));
    u64 allocCurAddr = (u64)allocCur;
    allocCurAddr += sizeof(FreeList);
    allocCur = (void*)allocCurAddr;
    InitFreeList(header, FALSE, size, allocSize, (u64)allocCur);
    
    void* prev = allocCur;
    allocCurAddr = (u64)allocCur;
    allocCurAddr += allocSize;
    allocCur = (void*)allocCurAddr;
    KeReleaseSpinLock(&allocSpinLock);
    return prev;
}

void MmSetAllowFrees(boolean value){
    allowFrees = value;
}



boolean MmFreeGeneralMemory(void* address){
    KeAcquireSpinLock(&allocSpinLock);
    boolean result = FreeMemory(address);
    KeReleaseSpinLock(&allocSpinLock);
    return result;
}