#include "kalloc.h"
#include "../hal/includes/mem.h"
#include <stdint.h>

#define PAGE_SIZE 0x1000

typedef struct _freeblock {
    struct _freeblock* next;
    uint64_t size;
} freeblock;

static void* freelist = NULL;
static freeblock* fblock = NULL;
static freeblock* initial = NULL;
static void* cur_memory = NULL;
static uint64_t cur_size = 0;

static inline uint8_t reinit_kalloc(){
    cur_memory = allocate_single_map();
    if(cur_memory == NULL){
        return 1;
    }
    cur_size = PAGE_SIZE;
    return 0;
}

void* kalloc(uint64_t size){
    static uint8_t init = 0;
    if(init == 0){
        if(reinit_kalloc() == 1){
            return NULL;
        }
        init = 1;
    }
    if(size > cur_size){
        if(reinit_kalloc() == 1){
            return NULL;
        }
    }
    void* allocated = cur_memory;
    cur_size -= size;
    cur_memory += size;
    return allocated;
}

// not impl, dont use
void kfree(void* address, uint64_t size){
    fblock = (freeblock*)address;
    fblock->size = size;
}
