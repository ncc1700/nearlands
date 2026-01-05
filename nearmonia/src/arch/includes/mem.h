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
    u64 mapKey;
} MemoryMap;


#define HHDM_OFFSET 0xFFFFFF8000000000ULL


MemoryMap* LdrMmRetrieveCurrentMemoryMap();
boolean LdrMmInitPhysicalAllocator();
void* LdrMmAllocatePhysicalPages(u64 amountOfPages);
void LdrMmFreePhysicalPages(void* pages, u64 amountOfPages);
void LdrMmUpdateCr3(u64 pml4Addr);
boolean LdrMmInitPaging();
boolean LdrMmMapHigherHalfMemoryForKernel(u64 address);

#endif