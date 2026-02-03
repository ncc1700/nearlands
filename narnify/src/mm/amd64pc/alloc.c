#include "ke/term.h"
#include "mm/pmm.h"
#include "types.h"
#include <mm/alloc.h>
#include <qol/qmem.h>


/**

    Bit better heap allocator, then the old one


    still scuffed but it..... works

*/

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

boolean MmInitGeneralAllocator(){
    allocBase = MmAllocateSinglePage();
    if(allocBase == NULL) return FALSE;
    allocCur = allocBase;
    allocLimit = allocBase + PAGE_SIZE;
    return TRUE;
}


void* MmAllocateGeneralMemory(u64 allocSize){
    u64 size = allocSize + sizeof(FreeList);

    if(size >= PAGE_SIZE){
        KeTermPrint(TERM_STATUS_INFO, QSTR("someone tried to allocate a ton of memory!"));
        // HACK! NOT GOOD
        // we just give it new pages away from the heap, but freeable and later
        // accessible by the heap
        u64 sizeInPages = (size + (PAGE_SIZE - 1)) / PAGE_SIZE;
        void* newPages = MmAllocateMultiplePages(sizeInPages);
        if(newPages == NULL){
            return NULL; // out of memory
        }
        FreeList* header = newPages;
        memset(header, 0, sizeof(FreeList));
        u64 allocCurAddr = (u64)newPages;
        allocCurAddr += sizeof(FreeList);
        void* addr = (void*)allocCurAddr;
        header->magic = FREELIST_HEADER_MAGIC;
        header->isFree = FALSE;
        header->address = (u64)addr;
        header->size = size;
        header->memSize = allocSize;
        header->header = header;
        KeTermPrint(TERM_STATUS_INFO, QSTR("returning 0x%x"), addr);
        return addr;
    }
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
                    header->magic = FREELIST_HEADER_MAGIC;
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
            header->magic = FREELIST_HEADER_MAGIC;
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
    header->magic = FREELIST_HEADER_MAGIC;
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

void MmSetAllowFrees(boolean value){
    allowFrees = value;
}

boolean MmFreeGeneralMemory(void* address){
    if(address == NULL) return FALSE;
    if(allowFrees == FALSE) return FALSE;
    FreeList* header = address - sizeof(FreeList);
    if(header == NULL) return FALSE;
    if(header->magic != FREELIST_HEADER_MAGIC){
        return FALSE;
    }
    header->isFree = TRUE;
    AddToFreeList(header);
    return TRUE;
}