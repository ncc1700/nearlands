#include "../../includes/mem.h"
#include <stdatomic.h>
#include <stdint.h>
#include "../../../limine.h"
#include "../../includes/spinlock.h"

#define PAGE_SIZE 0x1000

typedef struct _mapInfo {
    uint64_t address;
    uint64_t entry;
    uint8_t isFree;
} mapInfo;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    LIMINE_MEMMAP_REQUEST,
    0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    LIMINE_HHDM_REQUEST,
    0
};

static mapInfo* bmpinfo = NULL;
static uint64_t mapSize = 0;
static struct limine_memmap_response* memmap = NULL;
static struct limine_hhdm_response* hhdm = NULL;

uint8_t map_init(){
    if(memmap_request.response == NULL || hhdm_request.response == NULL){
        return 1;
    }
    memmap = memmap_request.response;
    hhdm = hhdm_request.response;

    for(uint64_t i = 0; i < memmap->entry_count; i++){
        if(memmap->entries[i]->type != LIMINE_MEMMAP_USABLE 
            && memmap->entries[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) continue;
        uint64_t entrySize = memmap->entries[i]->length;
        uint64_t base = memmap->entries[i]->base;
        for(uint64_t j = base; j < (base + entrySize); j+=PAGE_SIZE){
            
            mapSize++;
        }
    }
    for(uint64_t i = 0; i < memmap->entry_count; i++){
        if(memmap->entries[i]->type != LIMINE_MEMMAP_USABLE 
            && memmap->entries[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) continue;
        if(mapSize * sizeof(mapInfo) < memmap->entries[i]->length){
            bmpinfo = (mapInfo*)(hhdm->offset + memmap->entries[i]->base);
            memmap->entries[i]->base += mapSize * sizeof(mapInfo);
            memmap->entries[i]->length -= mapSize * sizeof(mapInfo);
            break;
        }
    }
    if(bmpinfo == NULL){
        return 1;
    }
    uint64_t k = 0;
    for(uint64_t i = 0; i < memmap->entry_count; i++){
        if(memmap->entries[i]->type != LIMINE_MEMMAP_USABLE 
            && memmap->entries[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) continue;
        uint64_t entrySize = memmap->entries[i]->length;
        uint64_t base = memmap->entries[i]->base;
        for(uint64_t j = base; j < (base + entrySize); j+=PAGE_SIZE){
            bmpinfo[k].address = (hhdm->offset + j);
            bmpinfo[k].isFree = 1; 
            bmpinfo[k].entry = i;
            k++;
        }
    }
    return 0;
}
static volatile atomic_flag alloc = ATOMIC_FLAG_INIT;
void* allocate_single_map(){
    sl_acquire(&alloc);
    for(uint64_t i = 0; i < mapSize; i++){
        if(bmpinfo[i].isFree == 1){
            bmpinfo[i].isFree = 0;
            sl_release(&alloc);
            return (void*)bmpinfo[i].address;
        }
    }
    sl_release(&alloc);
    // we ran out of memory =(
    return NULL;
}
static volatile atomic_flag free = ATOMIC_FLAG_INIT;
void free_single_map(void* address){
    sl_acquire(&free);
    for(uint64_t i = 0; i < mapSize; i++){
        if(bmpinfo[i].address == (uint64_t)address){
            if(bmpinfo[i].isFree == 1) return;
            bmpinfo[i].isFree = 1;
        }
    }
    sl_release(&free);
}

void* allocate_multiple_maps(uint64_t amount){
    sl_acquire(&alloc);
    for(uint64_t i = 0; i < mapSize; i++){
        if(i + amount >= mapSize) break;
        uint8_t isValid = 0;
        for(uint64_t j = 0; j < amount; j++){
            if(bmpinfo[i].entry != bmpinfo[i + j].entry){
                isValid = 0;
                break;
            }
            if(bmpinfo[i + j].isFree == 1){
                isValid = 1;
            } else {
                isValid = 0;
                break;
            }
        }
        if(isValid == 1){
            for(uint64_t j = 0; j < amount; j++){
                bmpinfo[i + j].isFree = 0;
            }
            sl_release(&alloc);
            return (void*)bmpinfo[i].address;
        } 
    }
    sl_release(&alloc);
    // we ran out of memory =(
    return NULL;
}

void free_multiple_maps(void* address, uint64_t amount){
    sl_acquire(&free);
    for(uint64_t i = 0; i < mapSize; i++){
        if(i + amount >= mapSize) break;
        for(int j = 0; j < amount; j++){
            if(bmpinfo[i].address == (uint64_t)address){
                if(bmpinfo[i + j].isFree == 1) return;
                bmpinfo[i + j].isFree = 1;
            }
        }   
    }
    sl_release(&free);
}