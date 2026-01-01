#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED











#include "../../types.h"

typedef enum _MemoryEntryTypes {
    MEM_TYPE_FREE,
    MEM_TYPE_USED,
    MEM_TYPE_ACPI
} MemoryEntryTypes;

typedef struct _MemoryMapEntry {
    uptr base;
    u64 size;
    MemoryEntryTypes type;
} MemoryMapEntry;

typedef struct _MemoryMap {
    MemoryMapEntry* memEntries;
    u64 amountOfEntries;
    u64 sizeOfEntireMemory;
} MemoryMap;



MemoryMap* LdrMmRetrieveCurrentMemoryMap();
boolean LdrMmInitPhysicalAllocator();
void* LdrMmAllocatePhysicalPages(u64 amountOfPages);
void LdrMmFreePhysicalPages(void* pages, u64 amountOfPages);
boolean LdrMmInitPaging();



#endif