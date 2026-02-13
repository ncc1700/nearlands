

#include "ke/spinlock.h"
#include "ke/term.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "nrstatus.h"
#include "types.h"
#include <mm/alloc.h>
#include <qol/qmem.h>
#include <stddef.h>

#ifdef USE_HOMEMADE_ALLOC
static SpinLock allocSpinLock = {0};

#define MEM_SPLIT 16


#define IN_SPLIT(x) (((x) + (MEM_SPLIT - 1)) / MEM_SPLIT)
#define SPLIT_SAFE(x) ((((x) + (MEM_SPLIT - 1)) / MEM_SPLIT) * MEM_SPLIT)

// yes, they're the same thing
// I seperated them just for
// sanity later on


typedef struct _BlockHeader {
    u64 base;
    u64 size;
    u64 amountUsed;
    i64 allocCounter;
    struct _BlockHeader* next;
    struct _BlockHeader* prev;
} BlockHeader;

typedef struct _AllocHeader {
    BlockHeader* header;
    u64 base;
    u64 size;
    struct _AllocHeader* next;
    struct _AllocHeader* prev;
    boolean isFree;
} AllocHeader;



static BlockHeader* bHead = NULL;
static BlockHeader* bTail = NULL;

static AllocHeader* aHead = NULL;
static AllocHeader* aTail = NULL;



static inline NearStatus AddBlockHeader(BlockHeader* header){
    if(bHead == NULL){
        bHead = header;
    }
    header->prev = bTail;
    
    if(bTail != NULL) bTail->next = header;
    bTail = header;
    header->next = NULL;
    return STATUS_SUCCESS;
}

static inline NearStatus RemoveBlockHeader(BlockHeader* header){
    BlockHeader* prev = header->prev;
    BlockHeader* next = header->next;
    if(bHead == header){
        bHead = next;
    }
    if(bTail == header){
        bTail = prev;
    }
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    header->prev = NULL;
    header->next = NULL;
    return STATUS_SUCCESS;
}

static inline NearStatus AddAllocHeader(AllocHeader* header){
    if(aHead == NULL){
        aHead = header;
    }
    header->prev = aTail;
    if(aTail != NULL) aTail->next = header;
    aTail = header;
    header->next = NULL;
    return STATUS_SUCCESS;
}

static inline NearStatus RemoveAllocHeader(AllocHeader* header){
    AllocHeader* prev = header->prev;
    AllocHeader* next = header->next;
    if(aHead == header){
        aHead = next;
    }
    if(aTail == header){
        aTail = prev;
    }
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    header->prev = NULL;
    header->next = NULL;
    return STATUS_SUCCESS;
}

static inline NearStatus CreateBlock(u64 allocSize){
    u64 headerSize = SPLIT_SAFE(sizeof(BlockHeader));
    u64 size = SPLIT_SAFE(allocSize + headerSize);

    u64 sizeInPages = IN_PAGES(size);
    void* mem = MmAllocateMultiplePages(sizeInPages);
    if(mem == NULL){
        return STATUS_OUT_OF_MEMORY;
    }
    BlockHeader* header = (BlockHeader*)mem;
    mem = (void*)((u64)mem + headerSize);
    header->size = size - headerSize;
    header->base = (u64)mem;
    header->amountUsed = 0;
    header->allocCounter = 0;
    AddBlockHeader(header);
    return STATUS_SUCCESS;
}

static inline NearStatus DeleteBlockHeader(BlockHeader* header){
    u64 headerSize = SPLIT_SAFE(sizeof(BlockHeader));
    u64 size = SPLIT_SAFE(header->size + headerSize);
    u64 sizeInPages = IN_PAGES(size);
    NearStatus status = RemoveBlockHeader(header);
    if(!NR_SUCCESS(status)){
        return status;
    }
    void* mem = (void*)(header->base - headerSize);
    status = MmFreeMultiplePages(mem, sizeInPages);
    return status;
}

// carvs reference (the minecraft youtuber)
static inline void* CarveMemoryFromBlock(BlockHeader* header, u64 size){
    void* mem = (void*)(header->base + header->amountUsed);
    header->amountUsed += size;
    return mem;
}

static inline void* AllocateFreshMemory(u64 allocSize){
    u64 aHeaderSize = SPLIT_SAFE(sizeof(AllocHeader));
    u64 size = SPLIT_SAFE(allocSize + aHeaderSize);
    BlockHeader* header = bHead;
    while(header != NULL){
        u64 remaining = header->size - header->amountUsed;
        if(remaining >= size){
            void* mem = CarveMemoryFromBlock(header, size);
            AllocHeader* aHeader = mem;
            mem = (void*)((u64)mem + aHeaderSize);
            aHeader->size = size - aHeaderSize;
            aHeader->base = (u64)mem;
            aHeader->isFree = FALSE;
            aHeader->header = header;
            header->allocCounter++;
            return mem;
        }
        header = header->next;
    }
    return NULL; // no valid blocks left =(
}

NearStatus MmInitGeneralAllocator(){
    allocSpinLock = KeCreateSpinLock();
    NearStatus status = CreateBlock(10);
    return status;
}

void* MmAllocateGeneralMemory(u64 allocSize){
    KeAcquireSpinLock(&allocSpinLock);
    void* mem = AllocateFreshMemory(allocSize);
    if(mem == NULL){
        NearStatus status = CreateBlock(allocSize + PAGE_SIZE);
        if(!NR_SUCCESS(status)){
            goto FAIL;
        }
    }
    mem = AllocateFreshMemory(allocSize);
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



NearStatus MmFreeGeneralMemory(void* address){
    KeAcquireSpinLock(&allocSpinLock);
    
    KeReleaseSpinLock(&allocSpinLock);
    return STATUS_SUCCESS;
}


#endif