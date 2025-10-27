#include "../../includes/mem.h"
#include "../../includes/spinlock.h"
#include "../../../core/klibc/string.h"

/**
    Stub
*/

// used for later
static atomic_flag flag = ATOMIC_FLAG_INIT;
static void* kernel_pml4 = NULL;

static inline uint64_t virt_to_phys(uint64_t virt) {
    return virt - get_hhdm()->offset;
}

static inline uint64_t phys_to_virt(uint64_t phys) {
    return phys + get_hhdm()->offset;
}

void setup_paging(){
    kernel_pml4 = allocate_single_map();
    if(kernel_pml4 == NULL){
        while(1){continue;}; // to replaced with panic when thats implemented
    }
    for(uint64_t i = 0; i < get_memmap()->entry_count; i++){
        
    }
    
    memset(kernel_pml4, 0, 0x1000);
}

