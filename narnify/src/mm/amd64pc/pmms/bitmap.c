#include "../../includes/pmm.h"






#ifdef BITMAP_PMM



static u64 pageCount = 0;
static u64 bitmapSize = 0;
static u8* bitmap = NULL;

#define BIT_OFFSET(b)  ((b) % sizeof(u8))


#define SET_BITMAP(bit) (bitmap[bit / 8] |= (1 << (bit % 8)))
#define CLEAR_BITMAP(bit) (bitmap[bit / 8] &= ~(1 << (bit % 8)))



boolean MmInitPhysicalMemoryManager(BootMemoryMap* memMap){
    // TODO: implement
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;
        if(memMap->memEntries[i].base == 0x0){
            memMap->memEntries[i].base += PAGE_SIZE;
            memMap->memEntries[i].size -= PAGE_SIZE;
        }
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        for(u64 j = base; j < (base + size); j+=PAGE_SIZE){
            pageCount++;
        }
    }
    bitmapSize = pageCount / 8;
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        if(size >= bitmapSize){
            bitmap = (u8*)base;
            memMap->memEntries[i].size -= size;
            memMap->memEntries[i].base += size;
            break;
        }
    }
    if(bitmap == NULL){
        return FALSE;
    }
    

    
    return FALSE;
}

void* MmAllocateSinglePage(){
    // TODO: implement
    return NULL;
}

void* MmAllocateMultiplePages(u64 amount){
    // TODO: implement
    return NULL;
}

boolean MmFreeSinglePage(void* pageAddress){
    // TODO: implement
    return NULL;
}

boolean MmFreeMultiplePages(void* pageAddress, u64 amount){
    // TODO: implement
    return NULL;
}


#endif