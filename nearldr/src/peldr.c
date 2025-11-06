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


#define FIELD_OFFSET(type, field) ((unsigned long long) __builtin_offsetof(type, field))
#define GetFirstSectionOfImage(h) ((PIMAGE_SECTION_HEADER) ((unsigned long)h+FIELD_OFFSET(IMAGE_NT_HEADERS64,OptionalHeader)+((PIMAGE_NT_HEADERS64)(h))->FileHeader.SizeOfOptionalHeader))


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

void peldr_load_image(Config conf, int mode, EFI_HANDLE image){
    wchar buf[512];
    atow(conf.kernel, buf, 512);
    qol_puts(buf);
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

    uint64_t imageAmountOfPages = (imageSize + 0xFFF) / 0x1000;

    EFI_STATUS status = qol_return_systab()->BootServices->AllocatePages(AllocateAddress,
                                                    EfiLoaderCode, imageAmountOfPages,
                                                    (EFI_PHYSICAL_ADDRESS*)(&imageBase));
    if(status != EFI_SUCCESS){
        qol_halt_system(L"Couldn't allocate pages for image");
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
    }
    LoaderInfo info = {
        1,
        10,
        memmap,
        {graphics_return_gop_info().fbAddress,
                graphics_return_gop_info().fbSize, graphics_return_gop_info().width,
                graphics_return_gop_info().height, graphics_return_gop_info().pixelPerScanLine}
    };

    load_sections(exebuf, imageBase, ntHeader, info);
    void (*EntryPoint)() = (void(*)())(imageBase + ntHeader->OptionalHeader.AddressOfEntryPoint);
    qol_return_systab()->BootServices->FreePool(conf.kernel);
    qol_return_systab()->BootServices->FreePool(conf.name);
    qol_return_systab()->BootServices->FreePool(exebuf);
    fs_close_file(file);
    qol_return_systab()->BootServices->ExitBootServices(image, mapKey);
    // "change da world, my final message, goodbye" - NearLDR
    EntryPoint();
}
