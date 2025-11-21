#include "peldr.h"
#include "arch/arch.h"
#include "config.h"
#include "externheaders/efi/SimpleFileSystem.h"
#include "externheaders/efi/Uefi.h"
#include "externheaders/efi/UefiBaseType.h"
#include "externheaders/efi/UefiMultiPhase.h"
#include "externheaders/efi/UefiSpec.h"
#include "fs.h"
#include "graphics.h"
#include "ldrconfig.h"
#include "qol.h"
#include "string.h"
#include <stdint.h>


typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned char BYTE;
typedef uint64_t ULONGLONG;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
#define IMAGE_SIZEOF_SHORT_NAME 8
typedef struct _IMAGE_DOS_HEADER {
    WORD   e_magic;
    WORD   e_cblp;
    WORD   e_cp;
    WORD   e_crlc;
    WORD   e_cparhdr;
    WORD   e_minalloc;
    WORD   e_maxalloc;
    WORD   e_ss;
    WORD   e_sp;
    WORD   e_csum;
    WORD   e_ip;
    WORD   e_cs;
    WORD   e_lfarlc;
    WORD   e_ovno;
    WORD   e_res[4];
    WORD   e_oemid;
    WORD   e_oeminfo;
    WORD   e_res2[10];
    LONG   e_lfanew;
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
  DWORD VirtualAddress;
  DWORD Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
  WORD                 Magic;
  BYTE                 MajorLinkerVersion;
  BYTE                 MinorLinkerVersion;
  DWORD                SizeOfCode;
  DWORD                SizeOfInitializedData;
  DWORD                SizeOfUninitializedData;
  DWORD                AddressOfEntryPoint;
  DWORD                BaseOfCode;
  ULONGLONG            ImageBase;
  DWORD                SectionAlignment;
  DWORD                FileAlignment;
  WORD                 MajorOperatingSystemVersion;
  WORD                 MinorOperatingSystemVersion;
  WORD                 MajorImageVersion;
  WORD                 MinorImageVersion;
  WORD                 MajorSubsystemVersion;
  WORD                 MinorSubsystemVersion;
  DWORD                Win32VersionValue;
  DWORD                SizeOfImage;
  DWORD                SizeOfHeaders;
  DWORD                CheckSum;
  WORD                 Subsystem;
  WORD                 DllCharacteristics;
  ULONGLONG            SizeOfStackReserve;
  ULONGLONG            SizeOfStackCommit;
  ULONGLONG            SizeOfHeapReserve;
  ULONGLONG            SizeOfHeapCommit;
  DWORD                LoaderFlags;
  DWORD                NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS64 {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

typedef struct _IMAGE_SECTION_HEADER {
    BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
            DWORD   PhysicalAddress;
            DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef struct _IMAGE_BASE_RELOCATION_BLOCK {
    uint32_t PageRVA;
    uint32_t BlockSize;
} IMAGE_BASE_RELOCATION_BLOCK;

#define FIELD_OFFSET(type, field) ((unsigned long long) __builtin_offsetof(type, field))
#define GetFirstSectionOfImage(h) ((PIMAGE_SECTION_HEADER) ((unsigned long)h+FIELD_OFFSET(IMAGE_NT_HEADERS64,OptionalHeader)+((PIMAGE_NT_HEADERS64)(h))->FileHeader.SizeOfOptionalHeader))

#define IMAGE_DIRECTORY_ENTRY_BASERELOC 5

#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_DIR64    10



static inline void load_sections(char* file,
    uint64_t imageBase,
    IMAGE_NT_HEADERS64* ntHeader,
    LoaderInfo config){
    PIMAGE_SECTION_HEADER sectionHeader = GetFirstSectionOfImage(ntHeader);

    for(int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++){
        memset((void*)(imageBase + sectionHeader->VirtualAddress),
                    0, sectionHeader->Misc.VirtualSize);
        //wchar buf[512];
        //atow((char*)sectionHeader->Name, buf, 512);
        ///qol_puts(buf);
        if(memcmp(sectionHeader->Name, ".ldr", 4) == 0){
            LoaderInfo* con = (void*)(imageBase + sectionHeader->VirtualAddress);
            con->fb.fbAddress = config.fb.fbAddress;
            con->fb.fbSize = config.fb.fbSize;
            con->fb.height = config.fb.height;
            con->fb.width = config.fb.width;
            con->fb.pixelPerScanLine = config.fb.pixelPerScanLine;
            con->memmap.amount = config.memmap.amount;
            con->memmap.memEntries = config.memmap.memEntries;
            con->memoryAmount = config.memoryAmount;
        } else {
            memcpy((void*)(imageBase + sectionHeader->VirtualAddress),
                    (void*)(file + sectionHeader->PointerToRawData), sectionHeader->SizeOfRawData);
        }

        sectionHeader++;
    }
}

static inline uint8_t relocate_image(uint64_t imageBase, IMAGE_NT_HEADERS64* ntHeader){
    uint64_t delta = imageBase - ntHeader->OptionalHeader.ImageBase;
    if(delta == 0) return 1;
    IMAGE_DATA_DIRECTORY relocDir = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    if(relocDir.VirtualAddress == 0 || relocDir.Size == 0) return 1;
    uint8_t* base = (uint8_t*)imageBase;
    uint8_t* relocStart = base + relocDir.VirtualAddress;
    uint8_t* relocEnd = relocStart + relocDir.Size;
    uint8_t* cur = relocStart;

    while(cur < relocEnd){
        IMAGE_BASE_RELOCATION_BLOCK* block = (IMAGE_BASE_RELOCATION_BLOCK*)cur;
        if(block->BlockSize < sizeof(IMAGE_BASE_RELOCATION_BLOCK)) break;
        uint64_t entryCount = (block->BlockSize - sizeof(IMAGE_BASE_RELOCATION_BLOCK)) / sizeof(uint16_t);
        uint16_t* entries = (uint16_t*)(cur + sizeof(IMAGE_BASE_RELOCATION_BLOCK));
        for(uint64_t i = 0; i < entryCount; i++){
            uint64_t entry = entries[i];
            uint64_t type = entry >> 12; // 15-12
            uint64_t offset = entry & 0x0FFF; // 11-0
            uint64_t* patchAddr64 = 0;
            uint32_t* patchAddr32 = 0;

            switch(type){
                case IMAGE_REL_BASED_DIR64:
                    patchAddr64 = (uint64_t*)(base + block->PageRVA + offset);
                    *patchAddr64 = (uint64_t)(*patchAddr64 + delta);
                    break;
                case IMAGE_REL_BASED_HIGHLOW:
                    patchAddr32 = (uint32_t*)(base + block->PageRVA + offset);
                    *patchAddr32 = (uint32_t)((uint64_t)(*patchAddr32) + (uint32_t)delta);
                    break;
                default:
                    break;
            }
        }
        cur += block->BlockSize;
    }
    ntHeader->OptionalHeader.ImageBase = imageBase;
    return 0;
}

void peldr_load_image(Config conf, int mode, EFI_HANDLE image){
    wchar buf[512];
    atow(conf.kernel, buf, 512);
    EFI_FILE_PROTOCOL* file = fs_open_file(buf, EFI_FILE_MODE_READ);
    if(file == NULL){
        qol_halt_system(L"Couldn't find image");
    }
    char* exebuf = fs_read_file(file);
    if(exebuf == NULL){
        qol_halt_system(L"error");
    }
    if(memcmp(exebuf, "MZ", 2) != 0){
        qol_halt_system(L"Not a PE Image, Failure during MZ Check");
    }
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)exebuf;
    IMAGE_NT_HEADERS64* ntHeader = (IMAGE_NT_HEADERS64*)(exebuf + dosHeader->e_lfanew);
    if(memcmp(ntHeader, "PE\0\0", 4) != 0){
        qol_halt_system(L"Not a PE Image, Failure during PE Check");
    }

    uint64_t imageBase = ntHeader->OptionalHeader.ImageBase;
    uint64_t imageSize = ntHeader->OptionalHeader.SizeOfImage;
    uint8_t shouldRelocate = 0;
    uint64_t imageAmountOfPages = (imageSize + 0xFFF) / 0x1000;

    EFI_STATUS status = qol_return_systab()->BootServices->AllocatePages(AllocateAddress,
                                                    EfiLoaderCode, imageAmountOfPages,
                                                    (EFI_PHYSICAL_ADDRESS*)(&imageBase));
    if(status != EFI_SUCCESS){
        qol_puts(L"couldn't allocate at PE specified ImageBase, allocating somewhere else");
        shouldRelocate = 1;
        // we allocate wherever we can, thanks to AllocateAnyPages
        status = qol_return_systab()->BootServices->AllocatePages(AllocateAnyPages,
            EfiLoaderCode, imageAmountOfPages,
            (EFI_PHYSICAL_ADDRESS*)&imageBase
        );
        if(status != EFI_SUCCESS){
            qol_halt_system(L"Not enough memory to load kernel");
        }
    }

    UINTN memMapSize;
    EFI_MEMORY_DESCRIPTOR* efiMemMap = NULL;
    UINTN mapKey = 0;
    UINTN descriptorSize = 0;
    UINT32 descriptorVersion = 0;
    // we don't error check here, why? we game by getting the size THEN allocating
    qol_return_systab()->BootServices->GetMemoryMap(&memMapSize, efiMemMap, &mapKey, &descriptorSize, &descriptorVersion);
    memMapSize += descriptorSize * 8;
    status = qol_return_systab()->BootServices->AllocatePool(EfiLoaderData, memMapSize, (void**)&efiMemMap);
    if(status != EFI_SUCCESS){
        qol_halt_system(L"Couldn't allocate memory for efi memmap");
    }
    status = qol_return_systab()->BootServices->GetMemoryMap(&memMapSize, efiMemMap, &mapKey, &descriptorSize, &descriptorVersion);
    if(status != EFI_SUCCESS){
        qol_halt_system(L"Couldn't get memory map");
    }
    uint64_t memsize = 0;
    uint64_t entries = memMapSize / descriptorSize;
    MemoryMap memmap = {NULL, 0};
    qol_return_systab()->BootServices->AllocatePool(EfiLoaderData, entries * sizeof(MemoryMapEntry), (void**)&memmap.memEntries);
    if(status != EFI_SUCCESS){
        qol_halt_system(L"Couldn't allocate memory for memmap");
    }
    memmap.amount = entries;
    for(int i = 0; i < entries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint8_t*)efiMemMap + (i * descriptorSize));

        memmap.memEntries[i].base = desc->PhysicalStart;
        memmap.memEntries[i].size = desc->NumberOfPages * 4096;
        memmap.memEntries[i].types = desc->Type;
        memsize += memmap.memEntries[i].size;
    }
    LoaderInfo info = {
        memsize,
        0,
        memmap,
        {graphics_return_gop_info().fbAddress,
                graphics_return_gop_info().fbSize, graphics_return_gop_info().width,
                graphics_return_gop_info().height, graphics_return_gop_info().pixelPerScanLine}
    };
    load_sections(exebuf, imageBase, ntHeader, info);
    if(shouldRelocate == 1){
        uint8_t result = relocate_image(imageBase, ntHeader);
        if(result == 1){
            qol_halt_system(L"Couldn't Relocate Executable!");
        }
    }
    void (*EntryPoint)() = (void(*)())(imageBase + ntHeader->OptionalHeader.AddressOfEntryPoint);
    qol_return_systab()->BootServices->FreePool(conf.kernel);
    qol_return_systab()->BootServices->FreePool(conf.name);
    qol_return_systab()->BootServices->FreePool(exebuf);
    fs_close_file(file);
    qol_return_systab()->BootServices->ExitBootServices(image, mapKey);
    // "change da world, my final message, goodbye" - NearLDR
    EntryPoint();
}
