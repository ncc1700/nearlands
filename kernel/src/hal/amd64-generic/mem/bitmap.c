#include "../../includes/mem.h"
#include <stdatomic.h>
#include <stdint.h>
#include "../../../limine.h"
#include "../../includes/spinlock.h"

#define PAGE_SIZE 0x1000

typedef struct _BitmapInfo {
    uint64_t address;
    uint64_t entry;
    uint8_t isFree;
} BitmapInfo;

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

static BitmapInfo* bmpinfo = NULL;
static uint64_t bitmapSize = 0;
static struct limine_memmap_response* memmap = NULL;
static struct limine_hhdm_response* hhdm = NULL;

uint8_t bitmap_init(){
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
            
            bitmapSize++;
        }
    }
    for(uint64_t i = 0; i < memmap->entry_count; i++){
        if(memmap->entries[i]->type != LIMINE_MEMMAP_USABLE 
            && memmap->entries[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) continue;
        if(bitmapSize * sizeof(BitmapInfo) < memmap->entries[i]->length){
            bmpinfo = (BitmapInfo*)(hhdm->offset + memmap->entries[i]->base);
            memmap->entries[i]->base += bitmapSize * sizeof(BitmapInfo);
            memmap->entries[i]->length -= bitmapSize * sizeof(BitmapInfo);
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
void* allocate_single_bitmap(){
    sl_acquire(&alloc);
    for(uint64_t i = 0; i < bitmapSize; i++){
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
void free_single_bitmap(void* address){
    sl_acquire(&free);
    for(uint64_t i = 0; i < bitmapSize; i++){
        if(bmpinfo[i].address == (uint64_t)address){
            if(bmpinfo[i].isFree == 1) return;
            bmpinfo[i].isFree = 1;
        }
    }
    sl_release(&free);
}

void* allocate_multiple_bitmaps(uint64_t amount){
    sl_acquire(&alloc);
    for(uint64_t i = 0; i < bitmapSize; i++){
        if(i + amount >= bitmapSize) break;
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

void free_multiple_bitmaps(void* address, uint64_t amount){
    sl_acquire(&free);
    for(uint64_t i = 0; i < bitmapSize; i++){
        if(i + amount >= bitmapSize) break;
        for(int j = 0; j < amount; j++){
            if(bmpinfo[i].address == (uint64_t)address){
                if(bmpinfo[i + j].isFree == 1) return;
                bmpinfo[i + j].isFree = 1;
            }
        }   
    }
    sl_release(&free);
}