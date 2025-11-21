#include "../includes/idmap.h"
#include "../../core/memory/physmem.h"
#include "../../core/klibc/string.h"

#include <stdint.h>

__attribute__((aligned(4096))) static uint64_t pml4[512] = {0};
__attribute__((aligned(4096))) static uint64_t pdpt[512] = {0};
__attribute__((aligned(4096))) static uint64_t* pd[512];



#define PG_PRESENT 0x1
#define PG_READ_WRITE 0x2
#define PG_PS 0x80
#define AMOUNT_OF_MEMORY_TO_MAP 512


extern void load_cr3(uint64_t pml4);
// maps 512gb of memory
void setup_identity_map(){
    
    pml4[0] = (uint64_t)pdpt | PG_PRESENT | PG_READ_WRITE;
    for(int i = 0; i < AMOUNT_OF_MEMORY_TO_MAP; i++){
        pd[i] = physmem_allocate_page();
        // gotta clear =(
        memset(pd[i], 0, 4096);
        pdpt[i] = (uint64_t)pd[i] | PG_PRESENT | PG_READ_WRITE;
    }   
    
    for(uint64_t i = 0; i < AMOUNT_OF_MEMORY_TO_MAP; i++){
        for(uint64_t j = 0; j < 512; j++){
            uint64_t physaddr = (i * 0x40000000) + (j * 0x200000);
            pd[i][j] = physaddr | PG_PRESENT | PG_READ_WRITE | PG_PS;
        }
    }
    load_cr3((uint64_t)pml4);
}