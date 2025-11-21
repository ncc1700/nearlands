#ifndef LDRCONFIG_H_INCLUDED
#define LDRCONFIG_H_INCLUDED








#include <stdint.h>

typedef enum {
    ///
    /// Not used.
    ///
    EfiReservedMemoryType,
    ///
    /// The code portions of a loaded application. 
    /// (Note that UEFI OS loaders are UEFI applications.)
    ///
    EfiLoaderCode,
    ///
    /// The data portions of a loaded application and the default data allocation
    /// type used by an application to allocate pool memory.
    ///
    EfiLoaderData,
    ///
    /// The code portions of a loaded Boot Services Driver.
    ///
    EfiBootServicesCode,
    ///
    /// The data portions of a loaded Boot Serves Driver, and the default data
    /// allocation type used by a Boot Services Driver to allocate pool memory.
    ///
    EfiBootServicesData,
    ///
    /// The code portions of a loaded Runtime Services Driver.
    ///
    EfiRuntimeServicesCode,
    ///
    /// The data portions of a loaded Runtime Services Driver and the default
    /// data allocation type used by a Runtime Services Driver to allocate pool memory.
    ///
    EfiRuntimeServicesData,
    ///
    /// Free (unallocated) memory.
    ///
    EfiConventionalMemory,
    ///
    /// Memory in which errors have been detected.
    ///
    EfiUnusableMemory,
    ///
    /// Memory that holds the ACPI tables.
    ///
    EfiACPIReclaimMemory,
    ///
    /// Address space reserved for use by the firmware.
    ///
    EfiACPIMemoryNVS,
    ///
    /// Used by system firmware to request that a memory-mapped IO region
    /// be mapped by the OS to a virtual address so it can be accessed by EFI runtime services.
    ///
    EfiMemoryMappedIO,
    ///
    /// System memory-mapped IO region that is used to translate memory
    /// cycles to IO cycles by the processor.
    ///
    EfiMemoryMappedIOPortSpace,
    ///
    /// Address space reserved by the firmware for code that is part of the processor.
    ///
    EfiPalCode,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;


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