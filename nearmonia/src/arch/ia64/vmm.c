#include "../includes/mem.h"
#include "../../qol.h"
#include "../../graphics.h"



__attribute__((aligned(4096))) static uptr pml4[512] = {0};



#define PG_PRESENT 0x1
#define PG_READ_WRITE 0x2
#define PG_PS 0x80

#define GET_PML4_ENTRY(x) ((x & ((u64)0x1ff << 39)) >> 39)
#define GET_PDPT_ENTRY(x) ((x & ((u64)0x1ff << 30)) >> 30)
#define GET_PD_ENTRY(x) ((x & ((u64)0x1ff << 21)) >> 21)
#define GET_PT_ENTRY(x) ((x & ((u64)0x1ff << 12)) >> 12)

#define PARSE_ENTRY_VALUE(x) (uptr*)(x & ~0xFFF)

static inline boolean GetNextLevel(uptr* address){
    // UNIMPLEMENTED
    return FALSE;
}

boolean LdrMmMapSinglePage(u64 address, u64 virtAddress){
    // UNIMPLEMENTED
    return FALSE;
}


boolean LdrMmInitPaging(){
    // UNIMPLEMENTED
    return FALSE;
}

boolean LdrMmMapHigherHalfMemoryForKernel(u64 address, u64 kernSize){
    // UNIMPLEMENTED
    return FALSE;
}