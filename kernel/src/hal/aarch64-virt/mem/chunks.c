/*
    Simple chunk allocator, pretty broken, needs to be fixed
*/

#include "../../includes/mem.h"
#include "../../../limine.h"
#include <stdint.h>
#include "../../../core/term/term.h"
#include "../../includes/spinlock.h"
#define CHUNK_SIZE 0x400

static struct limine_memmap_entry** memmap;
static uint64_t memmap_amount;
static atomic_flag lock = ATOMIC_FLAG_INIT;
#define ALIGN_UP(x, a) (((x) + (a - 1)) & ~(a - 1))

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

typedef struct _mem_detail {
    uint64_t loc_with_mem_detail;
    uint64_t base;
    uint64_t curIndex;
    uint64_t size;
} mem_detail;



static mem_detail* mem_det_map;
static uint64_t mem_det_size_all;

static void** free_list = NULL;


uint8_t chunk_allocator_setup(){
    if(memmap_request.response == NULL){
        return 1;
    }
    if(hhdm_request.response == NULL){
        return 2;
    }
    memmap = memmap_request.response->entries;
    memmap_amount = memmap_request.response->entry_count;
    mem_det_size_all = memmap_amount * sizeof(mem_detail);
    uint8_t setup = 0;
    uint64_t setup_index = 0;
    for(int i = 0; i < memmap_amount; i++){
        if(memmap[i]->type != LIMINE_MEMMAP_USABLE 
            && memmap[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) continue;
        if(memmap[i]->length <= mem_det_size_all){
            continue;
        }
        if(setup == 0){
            setup_index = i;
            mem_det_map = (mem_detail*)(memmap[i]->base + hhdm_request.response->offset);
            setup = 1;
        }
        
        if(i == setup_index){
            mem_det_map[i].base = (memmap[i]->base + hhdm_request.response->offset 
                                    + (memmap_amount * sizeof(mem_detail)));
            mem_det_map[i].loc_with_mem_detail = 1;
        } else {
            mem_det_map[i].base = (memmap[i]->base 
                                    + hhdm_request.response->offset);
            mem_det_map[i].loc_with_mem_detail = 0;
        }
        mem_det_map[i].curIndex = mem_det_map[i].base;
        mem_det_map[i].size = 0;
    }      
    return 0;
}

void* allocate_single_chunk(){
    sl_acquire(&lock);
    if(free_list){
        void* chunk = free_list;
        free_list = *(void**)free_list;
        sl_release(&lock);
        return chunk;
    }
    for(int i = 0; i < memmap_amount; i++){
        if(memmap[i]->type != LIMINE_MEMMAP_USABLE 
            && memmap[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
        {
            
            continue;
        } 
        uint64_t length;
        if(mem_det_map[i].loc_with_mem_detail == 1){
            length = (mem_det_map[i].curIndex - mem_det_map[i].base) + 
                                (memmap_amount * sizeof(mem_detail));  
        } else {
            length = (mem_det_map[i].curIndex - mem_det_map[i].base);
        }  
        if((length + CHUNK_SIZE) >= memmap[i]->length){
            
            continue;
        } 
        mem_det_map[i].curIndex = ALIGN_UP(mem_det_map[i].curIndex, 16);
        mem_det_map[i].curIndex += CHUNK_SIZE;
        uint64_t res = mem_det_map[i].curIndex;
        mem_det_map[i].size = (mem_det_map[i].curIndex - mem_det_map->base);
        sl_release(&lock);
        return (void*)res;
    }
    sl_release(&lock);
    return NULL;
}

void free_single_chunk(void* address){
    sl_acquire(&lock);
    *(void**)address = free_list;
    free_list = address;
    sl_release(&lock);
}

void* allocate_multiple_chunks(uint64_t amount){
    sl_acquire(&lock);
    if(free_list){
        void* start = free_list;
        void* cur = start;
        for(uint8_t i = 1; i < amount; i++){
            void* expected = (void*)((uint64_t)start + i * CHUNK_SIZE);
            void* next = *(void**)cur;
            if(next != cur){
                goto ALLOCATE_NEW;
            }
            cur = next;
        }
        free_list = *(void**)cur;
        sl_release(&lock);
        return start;
    }
    ALLOCATE_NEW:
    for(int i = 0; i < memmap_amount; i++){
        if(memmap[i]->type != LIMINE_MEMMAP_USABLE 
            && memmap[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
        {
            
            continue;
        } 
        uint64_t length;
        if(mem_det_map[i].loc_with_mem_detail == 1){
            length = (mem_det_map[i].curIndex - mem_det_map[i].base) + 
                                (memmap_amount * sizeof(mem_detail));  
        } else {
            length = (mem_det_map[i].curIndex - mem_det_map[i].base);
        }  
        if((length + CHUNK_SIZE) >= memmap[i]->length){
           
            continue;
        } 
        mem_det_map[i].curIndex = ALIGN_UP(mem_det_map[i].curIndex, 16);
        mem_det_map[i].curIndex += CHUNK_SIZE;
        uint64_t res = mem_det_map[i].curIndex;
        mem_det_map[i].size = (mem_det_map[i].curIndex - mem_det_map->base);
        sl_release(&lock);
        return (void*)res;
    }
    sl_release(&lock);
    return NULL;
}

void free_multiple_chunks(void* address, uint64_t amount){
    sl_acquire(&lock);
    for (uint64_t i = 0; i < amount; i++) {
        void* chunk = (void*)((uint64_t)address + i * CHUNK_SIZE);
        *(void**)chunk = free_list;
        free_list = chunk;
    }
    sl_release(&lock);
    return;
}