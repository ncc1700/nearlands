#include "ke/spinlock.h"
#include "ke/term.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "nrstatus.h"
#include "types.h"
#include <mm/alloc.h>
#include <qol/qmem.h>


/**

    Bit better heap allocator, then the old one


    still scuffed but it..... works

*/

static SpinLock allocSpinLock = {0};

#define ALLOCHEADER_MAGIC 0x6767
#define BLOCKHEADER_MAGIC 0x6161

typedef struct _FreeList {
    u64 address;
    struct _FreeList* next;
    struct _FreeList* prev;
    u64 size;
    u16 magic;
    boolean isFree;
} AllocHeader;

typedef struct _BlockHeader {
    struct _BlockHeader* next;
    struct _BlockHeader* prev;
    void* allocBase;
    void* allocCur;
    u16 magic;
    u64 allocTotalSize;
    u64 allocCurSize;
} BlockHeader;



static AllocHeader* flHead = NULL;
static AllocHeader* flTail = NULL;

static BlockHeader* bhHead = NULL;
static BlockHeader* bhTail = NULL;

//#define INCREMENT_MEMORY(addr, size)


static inline void AddToFreeList(AllocHeader* list){
    list->next = NULL;
    list->prev = flTail;
    if(flTail != NULL) flTail->next = list;
    flTail = list;
    if(flHead == NULL){
        flHead = list;
    } 
}

static inline void RemoveFromFreeList(AllocHeader* list){
    if(list == flHead){
        flHead = list->next;
    }
    if(list == flTail){
        flTail = list->prev;
    }
    AllocHeader* prev = list->prev;
    AllocHeader* next = list->next;
    
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    list->prev = NULL;
    list->next = NULL;
}

static inline void AddToBlockList(BlockHeader* list){
    list->next = NULL;
    list->prev = bhTail;
    if(bhTail != NULL) bhTail->next = list;
    bhTail = list;
    if(bhHead == NULL){
        bhHead = list;
    } 
}

static inline void RemoveFromBlockList(BlockHeader* list){
    if(list == bhHead){
        bhHead = list->next;
    }
    if(list == bhTail){
        bhTail = list->prev;
    }
    BlockHeader* prev = list->prev;
    BlockHeader* next = list->next;
    
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    list->prev = NULL;
    list->next = NULL;
}


static inline void InitAllocHeader(AllocHeader* header, boolean isFree, u64 size, u64 address)
{
    header->magic = ALLOCHEADER_MAGIC;
    header->isFree = isFree;
    header->address = (u64)address;
    header->size = size;
}

static inline void InitBlockHeader(BlockHeader* header, u64 size, u64 address)
{
    u64 trueAddr = (u64)address;
    trueAddr += sizeof(BlockHeader);
    header->magic = BLOCKHEADER_MAGIC;
    header->allocBase = (void*)trueAddr;
    header->allocCur = header->allocBase;
    header->allocTotalSize = size - sizeof(BlockHeader);
    header->allocCurSize = 0;
    AddToBlockList(header);
}

static inline BlockHeader* ReturnSuitableBlock(u64 size){
    BlockHeader* header = bhHead;
    while(header != NULL){
        u64 freeSize = header->allocTotalSize - header->allocCurSize;
        if(freeSize >= size){
            return header;
        }
        header = header->next;
    }
    return NULL; 
}

static inline void* AllocateFromBlock(BlockHeader* header, u64 size){
    u64 freeSize = header->allocTotalSize - header->allocCurSize;
    if(freeSize < size){
        return NULL;
    }
    void* addr = header->allocCur;
    header->allocCurSize += size;
    u64 curAddr = (u64)header->allocCur;
    curAddr += size;
    header->allocCur = (void*)curAddr;
    return addr;
}

static inline void* ReturnFreeMemory(u64 size){
    AllocHeader* header = flHead;
    while(header != NULL){
        if(header->size == size){
            RemoveFromFreeList(header);
            header->isFree = FALSE;
            return (void*)header->address;
        } else if(header->size >= (size + sizeof(AllocHeader))){
            RemoveFromFreeList(header);
            header->isFree = FALSE;
            u64 curAddr = header->address;
            u64 newFreeAddr = curAddr + size;
            AllocHeader* newHeader = (AllocHeader*)newFreeAddr;
            u64 remainingSize = (header->size - size) - sizeof(AllocHeader);
            InitAllocHeader(newHeader, TRUE, remainingSize, 
                    newFreeAddr + sizeof(AllocHeader));
            AddToFreeList(newHeader);

            header->size = size;
            return (void*)header->address;
        }
        header = header->next;
    }
    return NULL;
}

NearStatus MmInitGeneralAllocator(){
    void* initAlloc = MmAllocateSinglePage();
    if(initAlloc == NULL){
        return STATUS_OUT_OF_MEMORY;
    }
    BlockHeader* header = (BlockHeader*)initAlloc;
    InitBlockHeader(header, PAGE_SIZE, (u64)initAlloc);
    return STATUS_SUCCESS;
}

void* MmAllocateGeneralMemory(u64 allocSize){
    KeAcquireSpinLock(&allocSpinLock);
    AllocHeader* aHeader = NULL;
    void* result = NULL;
    u64 size = allocSize + sizeof(AllocHeader);

    result = ReturnFreeMemory(size);
    if(result != NULL){
        goto SUCCESS_EXIT;
    }

    BlockHeader* header = ReturnSuitableBlock(size);
    if(header == NULL){
        u64 sizeInPages = (size + (PAGE_SIZE - 1)) / PAGE_SIZE;
        
        void* initAlloc = MmAllocateMultiplePages(sizeInPages);

        if(initAlloc == NULL){
            goto FAIL_EXIT; // no more memory
        }
        BlockHeader* newHeader = (BlockHeader*)initAlloc;
        InitBlockHeader(newHeader, sizeInPages * PAGE_SIZE, 
                                            (u64)initAlloc);
    }
    header = ReturnSuitableBlock(size);
    if(header == NULL){
        goto FAIL_EXIT;
    }

    aHeader = AllocateFromBlock(header, size);
    if(aHeader == NULL){
        goto FAIL_EXIT;
    }
    result = (void*)((u64)aHeader + sizeof(AllocHeader));
    InitAllocHeader(aHeader, FALSE, allocSize, (u64)result);
SUCCESS_EXIT:
    KeReleaseSpinLock(&allocSpinLock);
    return result;
FAIL_EXIT:
    KeReleaseSpinLock(&allocSpinLock);
    return NULL;
}





NearStatus MmFreeGeneralMemory(void* address){
    KeAcquireSpinLock(&allocSpinLock);
    NearStatus status = STATUS_SUCCESS;
    if(address == NULL){
        status = STATUS_INVALID_ADDRESS;
        goto EXIT;
    }
    AllocHeader* header = (AllocHeader*)((u64)(address) - sizeof(AllocHeader));
    if(header->magic != ALLOCHEADER_MAGIC){
        status = STATUS_CANT_FIND_HEADER;
        goto EXIT;
    }
    if(header->isFree == TRUE){
        status = STATUS_INVALID_ADDRESS;
        goto EXIT;
    }
    header->isFree = TRUE;
    AddToFreeList(header);
EXIT:
    KeReleaseSpinLock(&allocSpinLock);
    return status;
}