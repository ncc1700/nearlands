#include "ke/spinlock.h"
#include "ke/term.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "nrstatus.h"
#include "types.h"
#include <mm/alloc.h>
#include <qol/qmem.h>

static SpinLock allocSpinLock = {0};

#define MEM_SPLIT 16



typedef struct _BlockHeader {
    struct _BlockHeader* next;
    struct _BlockHeader* prev;
    struct _BlockHeader* header;
    Arena* arena;
    u64 base;
    u64 cur; 
    u64 size;
    u64 curSize;
    u64 split;
} BlockHeader;

typedef struct _Freed {
    u64 address;
    u64 size;
} Freed;



static BlockHeader* head = NULL;
static BlockHeader* tail = NULL;

static Freed* freedArray = NULL;
static u64 freedCapacity = 0;
static u64 freedMaxCapacity = 256;

static inline void AddToBlockList(BlockHeader* list){
    list->next = NULL;
    list->prev = tail;
    if(tail != NULL) tail->next = list;
    tail = list;
    if(head == NULL){
        head = list;
    } 
}

static inline void RemoveFromBlockList(BlockHeader* list){
    if(list == head){
        head = list->next;
    }
    if(list == tail){
        tail = list->prev;
    }
    BlockHeader* prev = list->prev;
    BlockHeader* next = list->next;
    
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    list->prev = NULL;
    list->next = NULL;
}

static NearStatus CreateBlock(u64 pageAmount){
    void* mem = MmAllocateMultiplePages(pageAmount);
    if(mem == NULL){
        return STATUS_OUT_OF_MEMORY;
    }
    BlockHeader* header = (BlockHeader*)mem;
    u64 base = (u64)mem + sizeof(BlockHeader);
    u64 size = (pageAmount * PAGE_SIZE) - sizeof(BlockHeader);
    header->base = base;
    header->cur = header->base;
    header->curSize = 0;
    header->size = size;
    header->split = MEM_SPLIT;
    header->next = NULL;
    header->prev = NULL;
    AddToBlockList(header);
    return STATUS_SUCCESS;
}

static void* AllocateFromAvaliableBlock(u64 size){
    u64 trueAllocSize = ((size + (MEM_SPLIT - 1)) / MEM_SPLIT) * MEM_SPLIT;
    KeTermPrint(TERM_STATUS_INFO, QSTR("size: %d, trueAllocSize: %d"), size, trueAllocSize);
    BlockHeader* header = head;

    while(header != NULL){
        u64 sizeRemaining = header->size - header->curSize;
        if(sizeRemaining >= trueAllocSize){
            header->curSize += trueAllocSize;
            u64 base = header->cur;
            header->cur += trueAllocSize;
            return (void*)base;
        }
        header = header->next;
    }
    return NULL;
}

static NearStatus InitFreedArraySize(){
    u64 size = freedMaxCapacity * sizeof(Freed);
    u64 sizeInPages = ((size + 4095) / 4096);
    freedArray = MmAllocateMultiplePages(sizeInPages);
    if(freedArray == NULL){
        return STATUS_OUT_OF_MEMORY;
    }
    freedCapacity = 0;
    return STATUS_SUCCESS;
}

static NearStatus ExpandFreedArraySize(){
    u64 size = freedMaxCapacity * sizeof(Freed);
    u64 oldSizeInPages = ((size + 4095) / 4096);

    freedMaxCapacity = freedMaxCapacity * 2;
    size = freedMaxCapacity * sizeof(Freed);
    u64 sizeInPages = ((size + 4095) / 4096);

    freedArray = MmReallocatePages(freedArray, 
                        oldSizeInPages,
                        sizeInPages);
    if(freedArray == NULL){
        return STATUS_OUT_OF_MEMORY;
    }
    return STATUS_SUCCESS;
}

static void* ReturnMemoryFromFreedArray(u64 size){
    u64 trueAllocSize = ((size + (MEM_SPLIT - 1)) / MEM_SPLIT) * MEM_SPLIT;
    for(u64 i = 0; i < freedCapacity; i++){
        if(freedArray[i].size == trueAllocSize){
            u64 address = freedArray[i].address;
            freedArray[i].address = 0;
            freedArray[i].size = 0;
            return (void*)address;
        } else if(freedArray[i].size > trueAllocSize){
            u64 address = freedArray[i].address;
            freedArray[i].address += trueAllocSize;
            freedArray[i].size -= trueAllocSize;
            return (void*)address; 
        }
    }
    return NULL;
}

static NearStatus InsertFreedMemoryIntoArray(u64 address, u64 size){
    u64 trueAllocSize = ((size + (MEM_SPLIT - 1)) / MEM_SPLIT) * MEM_SPLIT;
    for(u64 i = 0; i < freedCapacity; i++){
        if(freedArray[i].size == 0 && freedArray[i].address == 0){
            freedArray[i].address = address;
            freedArray[i].size = trueAllocSize;
            return STATUS_SUCCESS;
        }
    }
    if(freedCapacity >= freedMaxCapacity){
        NearStatus status = ExpandFreedArraySize();
        if(!NR_SUCCESS(status)){
            return status;
        }
    }
    freedArray[freedCapacity].address = address;
    freedArray[freedCapacity].size = trueAllocSize;
    freedCapacity++;
    return STATUS_SUCCESS;
}

NearStatus MmInitGeneralAllocator(){
    allocSpinLock = KeCreateSpinLock();
    NearStatus status = InitFreedArraySize();
    if(!NR_SUCCESS(status)) return status;
    status = CreateBlock(1);
    return status;
}

void* MmAllocateGeneralMemory(u64 allocSize){
    KeAcquireSpinLock(&allocSpinLock);
    void* mem = NULL;
    // void* mem = ReturnMemoryFromFreedArray(allocSize);
    // if(mem != NULL){
    //     goto PASS;
    // }
    mem = AllocateFromAvaliableBlock(allocSize);
    if(mem == NULL){
        KeTermPrint(TERM_STATUS_INFO, QSTR("creating new block for kernel heap...."));
        u64 amountOfPagesForBlock = (((allocSize + sizeof(BlockHeader)) + 4095) / 4096) + 1;
        NearStatus status = CreateBlock(amountOfPagesForBlock);
        if(!NR_SUCCESS(status)){
            KeTermPrint(TERM_STATUS_INFO, QSTR("couldn't create new block!"));
            goto FAIL;
        }
    }
    // we try again, mimicks life =)
    mem = AllocateFromAvaliableBlock(allocSize);
    if(mem == NULL){
        KeTermPrint(TERM_STATUS_INFO, QSTR("couldn't find new block AGAIN"));
        goto FAIL;
    }
PASS:
    KeReleaseSpinLock(&allocSpinLock);
    return mem;
FAIL:
    KeReleaseSpinLock(&allocSpinLock);
    return NULL;
}

void MmSetAllowFrees(boolean value){
    //allowFrees = value;
    return;
}



NearStatus MmFreeGeneralMemory(void* address, u64 size){
    KeAcquireSpinLock(&allocSpinLock);
    if(address == NULL || size == 0){
        KeReleaseSpinLock(&allocSpinLock);
        return STATUS_INVALID_ADDRESS;
    }
    NearStatus status = InsertFreedMemoryIntoArray((u64)address, size);
    KeReleaseSpinLock(&allocSpinLock);
    return status;
}