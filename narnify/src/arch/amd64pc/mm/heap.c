#include "../../includes/heap.h"
#include "../../includes/pmm.h"




#define HEAP_BLOCK_SIZE 32

typedef struct _HeapBlock {
    u64 address;
    boolean isFree:1;
} __attribute__((packed)) HeapBlock;

static HeapBlock* heapBlocks = NULL;
static void* heap = NULL;
static u64 heapAmount = 0;


boolean MmInitHeapAllocator(u64 heapSize){
    u64 heapSizeInPages = (heapSize + (PAGE_SIZE - 1)) / PAGE_SIZE;
    heap = MmAllocateMultiplePages(heapSizeInPages);
    if(heap == NULL) return FALSE;
    for(u64 i = 0; i < heapSize; i+=HEAP_BLOCK_SIZE){
        heapAmount++;
    }
    u64 heapBlockSize = heapAmount * sizeof(HeapBlock);
    u64 heapBlockSizeInPages = (heapBlockSize + (PAGE_SIZE - 1)) / PAGE_SIZE;

    heapBlocks = MmAllocateMultiplePages(heapBlockSizeInPages);
    if(heapBlocks == NULL) return FALSE;
    u64 heapIndex = 0;
    for(u64 i = 0; i < heapSize; i+=HEAP_BLOCK_SIZE){
        heapBlocks[heapIndex].address = (u64)heap + i;
        heapBlocks[heapIndex].isFree = TRUE;
        heapIndex++;
    }
    return TRUE;
}


void* MmAllocateFromHeap(u64 size){
    u64 sizeAligned = (size + (HEAP_BLOCK_SIZE - 1)) & ~(HEAP_BLOCK_SIZE - 1);
    u64 heapBlockAmount = (sizeAligned + (HEAP_BLOCK_SIZE - 1)) / HEAP_BLOCK_SIZE;
    u64 indexToAlloc = 0;
    boolean shouldAlloc = FALSE;
    for(u64 i = 0; i < heapAmount; i++){
        if((i + heapBlockAmount) > heapAmount) break;
        if(heapBlocks[i].isFree == TRUE){
            u64 base = i;
            u64 prevAddress = 0;
            boolean good = TRUE;
            for(u64 j = 0; j < heapBlockAmount; j++){
                if(heapBlocks[base + j].isFree == FALSE){
                    good = FALSE;
                    break;
                }
                u64 curAddress = heapBlocks[base + j].address;
                if(prevAddress != 0){
                    if((curAddress - prevAddress) != HEAP_BLOCK_SIZE){
                        good = FALSE;
                        break;
                    }
                }
                prevAddress = curAddress;
            }
            if(good == TRUE){
                shouldAlloc = TRUE;
                indexToAlloc = i;
                break;
            }
        }
    }
    if(shouldAlloc == TRUE){
        for(u64 i = 0; i < heapBlockAmount; i++){
            heapBlocks[indexToAlloc + i].isFree = FALSE;
        }
        return (void*)heapBlocks[indexToAlloc].address;
    }
    return NULL;
}


boolean MmFreeFromHeap(void* address, u64 size){
    u64 sizeAligned = (size + (HEAP_BLOCK_SIZE - 1)) & ~(HEAP_BLOCK_SIZE - 1);
    u64 heapBlockAmount = (sizeAligned + (HEAP_BLOCK_SIZE - 1)) / HEAP_BLOCK_SIZE;
    for(u64 i = 0; i <= (heapAmount - heapBlockAmount); i++){
        if(heapBlocks[i].address == (u64)address){
            for(u64 j = 0; j < heapBlockAmount; j++){
                heapBlocks[i + j].isFree = TRUE;
            }
            return TRUE;
        }
    }
    return FALSE;
}