#include <mm/arena.h>
#include <mm/pmm.h>
#include <mm/heap.h>






boolean MmCreateArena(Arena* arena, u64 size){
    arena->size = size;
    arena->sizeInpages = (size + (PAGE_SIZE - 1)) / PAGE_SIZE;
    //arena->base = MmAllocateMultiplePages(arena->sizeInpages);
    arena->base = MmAllocateFromHeap(arena->size);
    if(arena->base == NULL) return FALSE;
    arena->used = 0;
    return TRUE;
}


void* MmPushMemoryFromArena(Arena* arena, u64 size){
    if(arena->base == NULL) return NULL;
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
    //boolean result = MmFreeMultiplePages(arena->base, arena->sizeInpages);
    boolean result = MmFreeFromHeap(arena->base, arena->size);
    if(result == FALSE) return FALSE;
    arena->size = 0;
    arena->sizeInpages = 0;
    arena->base = 0;
    return TRUE;
}