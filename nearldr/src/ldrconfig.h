#ifndef LDRCONFIG_H_INCLUDED
#define LDRCONFIG_H_INCLUDED








#include "externheaders/efi/UefiMultiPhase.h"
#include <stdint.h>


typedef struct _Framebuffer {
    uint64_t fbAddress;
    uint64_t fbSize;
    uint64_t width;
    uint64_t height;
    uint64_t pixelPerScanLine;
} Framebuffer;

typedef struct _MemoryMapEntry {
    uint64_t base;
    uint64_t size;
    EFI_MEMORY_TYPE types;
} MemoryMapEntry;

typedef struct _MemoryMap {
    MemoryMapEntry* memEntries;
    uint64_t amount;
} MemoryMap;

typedef struct _LoaderInfo {
    uint64_t memoryAmount;
    uint64_t mode;
    MemoryMap memmap;
    Framebuffer fb;
} LoaderInfo;





#endif