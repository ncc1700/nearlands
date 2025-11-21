#include "alloc.h"
#include "physmem.h"
#include <stdint.h>
#include <stddef.h>

/*
    More bad code but idc it works =D


*/


#define BLOCK_SIZE 8


typedef struct _CurMemory {
    uint8_t* mem;
    uint64_t size;
    uint64_t max;
    struct _CurMemory* next;
} CurMemory;

typedef struct _FreeMemory {
    uint64_t size;
    struct _FreeMemory* next;
} FreeMemory;

static CurMemory* list = NULL;
static CurMemory* initial = NULL;
static FreeMemory* freelist = NULL;

void* core_alloc(uint64_t size){
    if(size == 0) return NULL;

    FreeMemory** previous = &freelist;
    FreeMemory* current = freelist;
    while(current != NULL){
        if(current->size >= size){
            *previous = current->next;
            return (void*)(current + 1);
        }
        previous = &current->next;
        current = current->next;
    }

    uint64_t amount = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if(amount == 0) amount = 1;

    if(initial == NULL){
        list = (CurMemory*)physmem_allocate_page();
        list->mem = (uint8_t*)physmem_allocate_page();
        list->size = 0;
        list->max = 4096 / BLOCK_SIZE;
        list->next = NULL;
        initial = list;
    }
    if((list->size + amount) > list->max){
        list->next = (CurMemory*)physmem_allocate_page();
        list->next->mem = (uint8_t*)physmem_allocate_page();
        list->next->size = 0;
        list->next->max = 4096 / BLOCK_SIZE;
        list->next->next = NULL;
        list = list->next;
    }
    
    uint64_t curSize = list->size;
    list->size += amount;
    FreeMemory* header = (FreeMemory*)(list->mem + (curSize * BLOCK_SIZE));
    header->size = amount * BLOCK_SIZE; 
    header->next = NULL;
    
    return (void*)(header + 1);
}

void core_free(void* mem){
    if(mem == NULL) return;
    FreeMemory* header = ((FreeMemory*)mem) - 1;
    header->next = freelist;
    freelist = header;
}