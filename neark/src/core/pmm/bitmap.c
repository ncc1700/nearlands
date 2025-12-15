#include "bitmap.h"
#include "../klibc/string.h"
#include "../misc/panic.h"
#include "../kernterm/kernterm.h"

#include <stdint.h>






/**
 *  some of this was inspired from Polaris
 *  https://github.com/NSG650/Polaris
 *  Thanks to them!
 *
 *
 *  this doesn't work well currently, will fix
 *  will I probably use this unless I decide to fix it? Nah freeing doesn't work
 *
 *  the freemap is way more inefficient and hoggy but it works
 *
 */
static uint8_t* pmmBitmap = NULL;
static uint64_t pageAmount = 0;
static uint64_t lastUsed = 0;
static uint64_t freePages = 0;

#define PAGE_SIZE 4096

static inline uint8_t get(uint64_t bit){
    return pmmBitmap[bit / 8] & (1 << (bit % 8));
}

static inline void set(uint64_t bit){
    pmmBitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void reset(uint64_t bit){
    pmmBitmap[bit / 8] &= ~(1 << (bit % 8));
}

void bmp_init(MemoryMap* memmap){
    uint64_t highestAddr = 0;
    uint64_t amountOfPages = 0;
    // calculate the amount of useable pages
    for(uint64_t i = 0; i < memmap->amount; i++){
        if(memmap->memEntries[i].types == EfiConventionalMemory){
            uint64_t top = memmap->memEntries[i].base + memmap->memEntries[i].size;
            pageAmount += (memmap->memEntries[i].size / PAGE_SIZE);
        }
    }
    uint64_t bmp_size = pageAmount / 8;

    // find a place and store the bitmap
    for(uint64_t i = 0; i < memmap->amount; i++){
        if(memmap->memEntries[i].types == EfiConventionalMemory){
            if(memmap->memEntries[i].size >= bmp_size){
                pmmBitmap = (uint8_t*)(memmap->memEntries[i].base);
                memset(pmmBitmap, 0xFF, bmp_size);
                memmap->memEntries[i].base += bmp_size;
                memmap->memEntries[i].size -= bmp_size;
                break;
            }
        }
    }
    // set all pages to 0
    for(uint64_t i = 0; i < memmap->amount; i++){
        if(memmap->memEntries[i].types == EfiConventionalMemory){
            uint64_t basePage = memmap->memEntries[i].base / PAGE_SIZE;
            uint64_t amount = memmap->memEntries[i].size / PAGE_SIZE;
            for(uint64_t j = 0; j < amount; j++){
                reset(basePage + j);
            }
        }
    }
}

// stupid chungus function
void* bmp_allocate_pages_with_limit(uint64_t pages, uint64_t limit){
    uint64_t count = 0;
    for(uint64_t i = lastUsed; i < limit; i++){
        if(get(i) == 0){
            count++;
            if(count == pages){
                uint64_t start = i + 1 - pages;
                for (uint64_t j = 0; j < pages; j++) {
                    set(start + j);
                }
                lastUsed = start + pages;
                return (void*)(start * PAGE_SIZE);
            }
        } else count = 0;
    }
    return NULL; // ran out of memory =(
}

void* bmp_allocate_pages(uint64_t pages){
    uint64_t last = lastUsed;
    void* ret = bmp_allocate_pages_with_limit(pages, pageAmount);
    if(ret == NULL){
        lastUsed = 0;
        ret = bmp_allocate_pages_with_limit(pages, last);
        if(ret == NULL){
            core_panic("bmp_allocate_pages(): returned NULL");
            return NULL; // shouldn't enter here
        }
    }
    freePages -= pages;
    return ret;
}

void bmp_free_pages(void* mem, uint64_t pages){
    uint64_t memPage = (uint64_t)mem / PAGE_SIZE;
    for(uint64_t i = 0; i < pages; i++){
        reset(memPage + i);
    }
    freePages += pages;
}
