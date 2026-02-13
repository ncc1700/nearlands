

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
    u64 size = SPLIT_SAFE(allocSize);
    u64 headerSize = SPLIT_SAFE(sizeof(BlockHeader));
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

static inline void* GetMemoryFromFreeList(u64 allocSize){
    u64 aHeaderSize = SPLIT_SAFE(sizeof(AllocHeader));
    u64 size = SPLIT_SAFE(allocSize);
    AllocHeader* header = aHead;
    while(header != NULL){
        if(header->size == size || header->size == (size + aHeaderSize)){
            //KeTermPrint(TERM_STATUS_INFO, QSTR("[GENALLOC] reusing memory"));
            BlockHeader* bHeader = header->header;
            header->isFree = FALSE;
            bHeader->allocCounter++;
            RemoveAllocHeader(header);
            return (void*)(header->base);
        } else if(header->size > size){
            if(size <= aHeaderSize) goto CONTINUE;
            BlockHeader* bHeader = header->header;
            header->isFree = FALSE;
            bHeader->allocCounter++;
            u64 oldSize = header->size;
            header->size = size;
            RemoveAllocHeader(header);
            void* whatToReturn = (void*)(header->base);
            void* memAfter = (void*)(header->base + size);
            AllocHeader* newHeader = memAfter;
            memAfter = (void*)((u64)memAfter + aHeaderSize);
            newHeader->size = SPLIT_SAFE((oldSize - size) - aHeaderSize);
            newHeader->base = (u64)memAfter;
            newHeader->isFree = TRUE;
            newHeader->header = bHeader;
            AddAllocHeader(newHeader);
            return whatToReturn;
        }
        CONTINUE:
        header = header->next;
    }
    return NULL;
}

static inline void* AllocateFreshMemory(u64 allocSize){
    // TODO: fix this
    // void* freedMem = GetMemoryFromFreeList(allocSize);
    // if(freedMem != NULL){
    //     return freedMem;
    // }
    u64 aHeaderSize = SPLIT_SAFE(sizeof(AllocHeader));
    u64 size = SPLIT_SAFE(allocSize + aHeaderSize);
    BlockHeader* header = bHead;
    while(header != NULL){
        i64 remaining = header->size - header->amountUsed;
        if(remaining <= 0) remaining = 0;
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
    } else goto PASS;
    mem = AllocateFreshMemory(allocSize);
PASS:
    KeReleaseSpinLock(&allocSpinLock);
    return mem;
FAIL:
    KeTermPrint(TERM_STATUS_ERROR, QSTR("it failed??"));
    KeReleaseSpinLock(&allocSpinLock);
    return NULL;
}

void MmSetAllowFrees(boolean value){
    //allowFrees = value;
    return;
}



NearStatus MmFreeGeneralMemory(void* address){
    NearStatus status = STATUS_SUCCESS;
    KeAcquireSpinLock(&allocSpinLock);
    if(address == NULL) {
        status = STATUS_INVALID_ADDRESS;
        goto EXIT;
    }
    u64 aHeaderSize = SPLIT_SAFE(sizeof(AllocHeader));
    AllocHeader* aHeader = (AllocHeader*)((u64)address - aHeaderSize);
    if(aHeader->isFree == TRUE){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("[GENALLOC] someone tried to double free!!"));
        status = STATUS_ACCESS_DENIED;
        goto EXIT;
    }
    BlockHeader* bHeader = aHeader->header;
    bHeader->allocCounter--;
    if(bHeader->allocCounter <= 0){
        // not so optimized
        // TODO: optimize this WAY more
        // maybe group allocheaders
        // via their blockheaders
        AllocHeader* aHeaderL = aHead;
        while(aHeaderL != NULL){
            if(aHeaderL->header == bHeader){
                AllocHeader* header = aHeaderL;
                aHeaderL = aHeaderL->next;
                RemoveAllocHeader(header);
                continue;
            }
            aHeaderL = aHeaderL->next;
        }
        DeleteBlockHeader(bHeader);
    } else {
        aHeader->isFree = TRUE;
        AddAllocHeader(aHeader);
    }
EXIT:
    KeReleaseSpinLock(&allocSpinLock);
    return status;
}


#endif