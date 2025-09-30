#include "../../includes/mem.h"
#include "../../../limine.h"
#include <stdint.h>
#include "../../../core/tools/debugtools.h"
#include "../../includes/misc.h"


static uint64_t curMem = 0;

static struct limine_memmap_entry** memmap;
static uint64_t memmap_amount;

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

uint8_t chunk_allocator_setup(){
    if(memmap_request.response == NULL){
        return 1;
    }
    if(hhdm_request.response == NULL){
        return 2;
    }
    memmap = memmap_request.response->entries;
    memmap_amount = memmap_request.response->entry_count;
    curMem = 0;
    return 0;
}

void* allocate_single_chunk(){
    for(int i = 0; i < memmap_amount; i++){
        if(memmap[i]->type != LIMINE_MEMMAP_USABLE 
            && memmap[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) continue;

        uint64_t memaddr = memmap[i]->base + hhdm_request.response->offset;
        for(uint64_t i = memaddr; i < curMem; i+=0x400){
            if(*(volatile uint64_t*)i == 0){
                *(volatile uint64_t*)i = 1;
                uint64_t res = i + 8;
                return (void*)res;
            }
        }
        // if(curMem >= memmap[i]->length){
        //     curMem = 0;
        //     continue;
        // }
        if(curMem == 0){
            curMem = memaddr;
            *(volatile uint64_t*)curMem = 0;
        } 
        curMem += 0x400;
        *(volatile uint64_t*)curMem = 1;
        uint64_t res = curMem + 8;
        return (void*)res;
    }
    return NULL;
}

void free_single_chunk(void* address){
    *(volatile uint64_t*)(address - 8) = 0; 
}

void* allocate_multiple_chunks(uint64_t amount){
    // for(uint64_t i = memaddr; i < curMem; i+=0x400){
    //     if(*(volatile uint64_t*)i == 0){
    //         uint64_t valid = 1;
    //         for(uint64_t j = i; j < i + amount * 0x400; j+=0x400){
    //             if(*(volatile uint64_t*)j != 0){
    //                 valid = 0;
    //                 break;
    //             }
    //         }
    //         if(valid != 1) continue;
    //         for(uint64_t j = i; j < i + amount * 0x400; j+=0x400){
    //             *(volatile uint64_t*)j = 1;
    //         }
    //         return (void*)i + 8;
    //     }
    // }
    // uint64_t return_value = curMem += 0x400;
    // for (uint64_t j = 0; j < amount; j++) {
    //     *(volatile uint64_t*)curMem = 1;
    //     curMem += 0x400;
    // }
    // return (void*)return_value + 8;
}

void free_multiple_chunks(void* address, uint64_t amount){
    for(uint64_t i = (uint64_t)address; i < (uint64_t)address + amount * 0x400; i+=0x400){
        *(volatile uint64_t*)(i - 8) = 0; 
    }
}