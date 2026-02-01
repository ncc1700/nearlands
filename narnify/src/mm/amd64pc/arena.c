#include "ke/term.h"
#include "mm/alloc.h"
#include <mm/arena.h>
#include <mm/pmm.h>






boolean MmCreateArena(Arena* arena, u64 size){
    arena->size = size;
    arena->sizeInpages = (size + (PAGE_SIZE - 1)) / PAGE_SIZE;
    //arena->base = MmAllocateMultiplePages(arena->sizeInpages);
    arena->base = MmAllocateGeneralMemory(arena->size);
    if(arena->base == NULL){
        KeTermPrint(TERM_STATUS_INFO, QSTR("failed to create arena of size %d"), size);
        return FALSE;
    }
    arena->used = 0;
    return TRUE;
}


void* MmPushMemoryFromArena(Arena* arena, u64 size){
    if(arena->base == NULL){
        KeTermPrint(TERM_STATUS_INFO, QSTR("arena base is NULL"));
        return NULL;
    }
    if((arena->used + size) > arena->size){
        KeTermPrint(TERM_STATUS_INFO, QSTR("arena overflow, used is %d, size is %d"),
                               (size), arena->size);
        return NULL;
    }
    u64 usedBeforeAdd = arena->used;
    arena->used += size;
    return (void*)((u8*)arena->base + usedBeforeAdd);
}


void MmPopMemoryFromArena(Arena* arena, u64 size){
    if(size >= arena->used) arena->used = 0;
    else arena->used -= size;
}

void MmResetArena(Arena* arena){
    arena->used = 0;
}

boolean MmDestroyArena(Arena* arena){
    //boolean result = MmFreeMultiplePages(arena->base, arena->sizeInpages);
    boolean result = MmFreeGeneralMemory(arena->base);
    if(result == FALSE) return FALSE;
    arena->size = 0;
    arena->sizeInpages = 0;
    arena->base = 0;
    return TRUE;
}