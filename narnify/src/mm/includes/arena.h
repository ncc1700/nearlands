#ifndef ARENA_H_INCLUDED
#define ARENA_H_INCLUDED



#include "../../types.h"




typedef struct _Arena {
    void* base;
    u64 size;
    u32 sizeInpages;
    u64 used;
} Arena;




boolean MmCreateArena(Arena* arena, u64 size);
void* MmPushMemoryFromArena(Arena* arena, u64 size);
void MmPopMemoryFromArena(Arena* arena, u64 size);
void MmResetArena(Arena* arena);
boolean MmDestroyArena(Arena* arena);














#endif