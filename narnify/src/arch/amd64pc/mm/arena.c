#include "../../includes/arena.h"
#include "../../includes/pmm.h"






boolean MmCreateArena(Arena* arena, u64 size){
    arena->size = size;
    arena->sizeInpages = (size + (PAGE_SIZE - 1)) / PAGE_SIZE;
    arena->base = MmAllocateMultiplePages(arena->sizeInpages);
    if(arena->base == NULL) return FALSE;
    arena->used = 0;
    return TRUE;
}


void* MmPushMemoryFromArena(Arena* arena, u64 size){
    if((arena->used + size) > arena->size) return NULL;
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
    boolean result = MmFreeMultiplePages(arena->base, arena->sizeInpages);
    if(result == FALSE) return FALSE;
    arena->size = 0;
    arena->sizeInpages = 0;
    arena->base = 0;
    return TRUE;
}