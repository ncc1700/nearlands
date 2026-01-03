#ifndef BOOTINFO_H_INCLUDED
#define BOOTINFO_H_INCLUDED



#include "types.h"


typedef enum _BootMemoryEntryTypes {
    BOOT_MEM_TYPE_FREE,
    BOOT_MEM_TYPE_USED,
    BOOT_MEM_TYPE_ACPI
} BootMemoryEntryTypes;

typedef struct _BootMemoryMapEntry {
    uptr base;
    u64 size;
    BootMemoryEntryTypes type;
} BootMemoryMapEntry;

typedef struct _BootMemoryMap {
    BootMemoryMapEntry* memEntries;
    u64 amountOfEntries;
    u64 sizeOfEntireMemory;
    u64 mapKey;
} BootMemoryMap;

typedef struct _BootGraphicsData {
    boolean init;
    u64 framebufferBase;
    u64 framebufferSize;
    u32 width;
    u32 height;
    u32 pixelsPerScanLine;
} BootGraphicsData;



typedef struct _BootInfo {
    BootGraphicsData graphicsData;
    BootMemoryMap* memMap;
    u64 hhdmOffset;
    u64 kernelLocVirt;
    u64 kernelLocPhys;
    u64 typeOfBoot;
} BootInfo;
















#endif