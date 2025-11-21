#include "physmem.h"
#include <stddef.h>

/*
 * WARNING BAD CODE
 *
 *
 * very bad code
 *
 * bad code
 * bad
 * ndkenjlkfnjlkdenelndejkdnelkndklenejdklendlknejlndk
 *
 * at least it works =) (sometimes)
 *
 *
 */

static MemoryMap stMemmap = {0};
static uint64_t memAmount = 0;;

typedef struct _FreeMap {
    uint64_t base;
    uint8_t isFree;
} FreeMap;

static FreeMap* freemap = NULL;
static uint64_t freeMapAmount = 0;

void initialize_physmem_allocator(MemoryMap* memmap, uint64_t memory_amount){
    stMemmap.amount = memmap->amount;
    stMemmap.memEntries = memmap->memEntries;
    memAmount = memory_amount;
    uint64_t freemapamount = 0;
    // we start the many scans of memory we will be doing =)
    // very bad idea but idrc

    // this one is to see how large the freemap will be
    for(int i = 0; i < stMemmap.amount; i++){
        if(stMemmap.memEntries[i].types != EfiConventionalMemory) continue;
        freemapamount += (stMemmap.memEntries[i].size / 4096); // the amount of pages per entry
    }
    freeMapAmount = freemapamount;
    uint64_t freemapSize = freemapamount * sizeof(FreeMap);
    // this one is to find a suitable place to hold the freemap
    for(int i = 0; i < stMemmap.amount; i++){
        MemoryMapEntry* map = &stMemmap.memEntries[i];

        if(stMemmap.memEntries[i].types != EfiConventionalMemory) continue;
        if(map->size > freemapSize){
            freemap = (FreeMap*)map->base;
            map->base += freemapSize;
            map->size -= freemapSize;
            map->types = EfiLoaderData;
            break;
        }
    }
    // and then we fill the freemap by yet again looping
    uint64_t index = 0;
    for(int i = 0; i < stMemmap.amount; i++){
        if(stMemmap.memEntries[i].types != EfiConventionalMemory) continue;
        uint64_t size = stMemmap.memEntries[i].base + stMemmap.memEntries[i].size;
        for(uint64_t j = stMemmap.memEntries[i].base; j < size; j +=4096){
            freemap[index].base = j;
            freemap[index].isFree = 1;
            index++;
        }
    }
    // some safety stuff
    uint64_t freemapPages = (freemapSize + 4095) / 4096;
    for (int i = 0; i < freemapPages; i++) {
        freemap[i].isFree = 0;
    }
}


void* physmem_allocate_page(){
    for(uint64_t i = 0; i < freeMapAmount; i++){
        if(freemap[i].isFree == 1){
            freemap[i].isFree = 0;
            return (void*)freemap[i].base;
        }
    }
    return NULL;
}

void* physmem_allocate_multiple_pages(uint64_t amount){
    for(uint64_t i = 0; i < freeMapAmount; i++){
        if((freeMapAmount - i) < amount) continue;
        if(freemap[i].isFree == 1){
            uint8_t shouldBreak = 0;
            for(int j = 0; j < amount; j++){
                if(freemap[i + j].isFree != 1){
                    shouldBreak = 1;
                    break;
                }
            }
            if(shouldBreak == 1) continue;
            for(int j = 0; j < amount; j++){
                freemap[i + j].isFree = 0;
            }
            return (void*)freemap[i].base;
        }
    }
    return NULL;
}



// VERY BAD FREE PAGE IMPL
// TOO LAZY TO MAKE BETTER
void physmem_free_page(void* mem){
    for(uint64_t i = 0; i < freeMapAmount; i++){
        if(freemap[i].base == (uint64_t)mem){
            freemap[i].isFree = 1;
            break;
        }
    }
}


void physmem_free_multiple_pages(void* mem, uint64_t amount){
    for(uint64_t i = 0; i < freeMapAmount; i++){
        if((freeMapAmount - i) < amount) continue;
        if(freemap[i].base == (uint64_t)mem){
            for(int j = 0; j < amount; j++){
                freemap[i + j].isFree = 1;
            }
            break;
        }
    }
}
