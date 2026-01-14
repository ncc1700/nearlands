#include "peldr.h"
#include "arch/includes/mem.h"
#include "extern/EFI/SimpleFileSystem.h"
#include "extern/EFI/UefiBaseType.h"
#include "extern/EFI/UefiMultiPhase.h"
#include "extern/EFI/UefiSpec.h"
#include "fs.h"
#include "graphics.h"
#include "qol.h"
#include "term.h"
#include "bootinfo.h"

// stole these structs from MSDN/MS Learn and some other sites
// thanks to them!
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
#define GetFirstSectionOfImage(h) ((PIMAGE_SECTION_HEADER) ((u64)h+FIELD_OFFSET(IMAGE_NT_HEADERS64,OptionalHeader)+((PIMAGE_NT_HEADERS64)(h))->FileHeader.SizeOfOptionalHeader))



static inline void LoadSectionsIntoMemory(char* content, IMAGE_NT_HEADERS64* ntHeader, BootInfo* bInfo){
    u64 imageBase = ntHeader->OptionalHeader.ImageBase;
    IMAGE_SECTION_HEADER* secHeader = GetFirstSectionOfImage(ntHeader);
    for(int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++){
        memset((void*)(imageBase + secHeader->VirtualAddress), 0, 
                                secHeader->Misc.VirtualSize);
        TermPrint(TERM_STATUS_INFO, "Loading section %s", secHeader->Name);
        if(memcmp(secHeader->Name, ".ldr", 4) == 0){
            memcpy((void*)(imageBase + secHeader->VirtualAddress), 
                        (void*)(bInfo), sizeof(BootInfo));
        } else {
            memcpy((void*)(imageBase + secHeader->VirtualAddress), 
                        (void*)(content + secHeader->PointerToRawData), secHeader->SizeOfRawData);
        }
        secHeader++;
    }
}



void LdrPeLoadPEImageAsKernel(const char* path){
    EFI_FILE_PROTOCOL* kernFile = LdrFsOpenFile(path, EFI_FILE_MODE_READ);
    if(kernFile == NULL){
        QolPanic("Couldn't load kernel image! Please Reboot");
    }
    u64 length = 0; // we will probably never use this
    char* content = LdrFsReadFileContent(kernFile, &length);
    LdrFsCloseFile(kernFile);
    if(content == NULL){
        QolPanic("Kernel image is empty or corrupted! Please Reboot");
    }
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)content;
    if(memcmp(dosHeader, "MZ", 2) != 0){
        QolPanic("Kernel image isn't of PE type! (failed at MZ) Please Reboot");
    }
    IMAGE_NT_HEADERS64* ntHeader = (IMAGE_NT_HEADERS64*)(content + dosHeader->e_lfanew);
    if(memcmp(ntHeader, "PE\0\0", 4) != 0){
        QolPanic("Kernel image isn't of PE type! (failed at PE) Please Reboot");
    }
    TermPrint(TERM_STATUS_PASS, "%s is a valid kernel!\n", path);

    u64 sizeOfImage = ntHeader->OptionalHeader.SizeOfImage;
    u64 imageBase = ntHeader->OptionalHeader.ImageBase;
    uint64_t amountOfPagesForImage = (sizeOfImage + 4095) / 4096;
    u64 physBase = 0x0;
    EFI_STATUS status = QolReturnSystemTable()->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, 
                                                            amountOfPagesForImage, &physBase);
    if(status != EFI_SUCCESS){
        QolPanic("Not enough memory to load kernel!");
    }
    boolean result = LdrMmMapHigherHalfMemoryForKernel(physBase, amountOfPagesForImage * 4096);
    if(result == FALSE){
        QolPanic("Couldn't map higher half memory pages for kernel!");
    }
    TermPrint(TERM_STATUS_PASS, "Kernel physical base is located at 0x%x, page size is %d", physBase, amountOfPagesForImage);
    void (*KernelEntry)() = (void (*)())(imageBase + ntHeader->OptionalHeader.AddressOfEntryPoint);
    

    
    BootInfo bInfo = {0};
    bInfo.graphicsData = (BootGraphicsData){
        GraphicsReturnData()->init,
        GraphicsReturnData()->framebufferBase,
        GraphicsReturnData()->framebufferSize,
        GraphicsReturnData()->width,
        GraphicsReturnData()->height,
        GraphicsReturnData()->pixelsPerScanLine
    };
    bInfo.hhdmOffset = HHDM_OFFSET;
    bInfo.typeOfBoot = 0;
    MemoryMap* memmap = LdrMmRetrieveCurrentMemoryMap();
    bInfo.kernelLocPhys = physBase;
    bInfo.kernelLocVirt = HHDM_OFFSET;
    bInfo.kernelSizeInPages = amountOfPagesForImage;
    bInfo.memMap = (BootMemoryMap*)memmap;

    LoadSectionsIntoMemory(content, ntHeader, &bInfo);

    

    TermPrint(TERM_STATUS_PASS, "About to jump to kernel!");
    TermPrint(TERM_STATUS_INFO, "BootInfo is located at 0x%x", bInfo);
    TermPrint(TERM_STATUS_UNKNOWN, " ");
    TermPrint(TERM_STATUS_UNKNOWN, " ");
    QolReturnSystemTable()->BootServices->ExitBootServices(QolReturnImagehandle(), memmap->mapKey);
    // goodbye! change da world - Nearmonia to Narnify
    KernelEntry();
    QolPanic("Kernel returned from entrypoint!");
    return;
    
}