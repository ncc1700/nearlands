#include "elfldr.h"
#include "externheaders/efi/SimpleFileSystem.h"
#include "externheaders/efi/UefiBaseType.h"
#include "externheaders/efi/UefiMultiPhase.h"
#include "externheaders/efi/UefiSpec.h"
#include "fs.h"
#include "graphics.h"
#include "ldrconfig.h"
#include "qol.h"
#include "string.h"
#include <stdint.h>





#define ELF_MAGIC "\177ELF"

#define SECHEAD_NULL 0
#define SECHEAD_PROGRAM_DATA 1
#define SECHEAD_SYMBOL_TABLE 2
#define SECHEAD_STRING_TABLE 3
#define SECHEAD_RELOCATE_WITH_ADDENDS 4
#define SECHEAD_RELOCATE_NO_ADDENDS 9


#define PROGHEAD_LOAD 1

#define SECHEAD_NULL 0
#define SECHEAD_PROGRAM_DATA 1
#define SECHEAD_SYMBOL_TABLE 2
#define SECHEAD_STRING_TABLE 3
#define SECHEAD_RELOCATE_WITH_ADDENDS 4
#define SECHEAD_RELOCATE_NO_ADDENDS 9

typedef struct __attribute__((packed)) _ElfHeader {
    unsigned char magic[16];
    uint16_t type;
    uint16_t architecture;
    uint32_t version;
    uint64_t entrypoint;
    uint64_t progHeaderOffset;
    uint64_t secHeaderOffset;
    uint32_t cpuFlags;
    uint16_t elfHeaderSize;
    uint16_t progHeaderTableSize;
    uint16_t progHeaderTableCount;
    uint16_t secHeaderTableSize;
    uint16_t secHeaderTableCount;
    uint16_t secHeaderStrTableIndex;
} ElfHeader;


typedef struct __attribute__((packed)) _ElfSecHeader {
    uint32_t secName;
    uint32_t secType;
    uint64_t secFlags;
    uint64_t secVAddr;
    uint64_t secOffset;
    uint64_t secSize;
    uint32_t secLinkToOtherSec;
    uint32_t secInfo;
    uint64_t secAddrAlign;
    uint64_t secEntrySize;
} ElfSecHeader;


typedef struct __attribute__((packed)) _ElfProgramHeader {
    uint32_t programType;
    uint32_t programFlags;
    uint64_t programOffset;
    uint64_t programVAddr;
    uint64_t programPAddr;
    uint64_t programFileSize;
    uint64_t programMemorySize;
    uint64_t programAlignment;
} ElfProgramHeader;

typedef struct {
    uint64_t offset;
    uint64_t info;
    int64_t  addend;
} ElfRelocation;


#define AARCH64_REL 1027
// idk if we will ever use it, but if we do need it its
// good to have
typedef struct _ElfDynamicLoading {
    uint64_t dynEntryType;
    union {
        uint64_t dynIntValue;
        uint64_t dynAddressValue;
    } dynUnion;
} ElfDynamicLoading;




static inline char* get_elf_section_name(char* imageBuffer, ElfHeader* header, ElfSecHeader* sHeader){
    ElfSecHeader* firstSecHeader = (ElfSecHeader*)(imageBuffer + header->secHeaderOffset);
    ElfSecHeader* secStrLand = (ElfSecHeader*)&firstSecHeader[header->secHeaderStrTableIndex];
    return (char*)((imageBuffer + secStrLand->secOffset) + sHeader->secName);
}  



static inline uint64_t address_to_allocate_to(char* imgbuf, ElfHeader* header){
    ElfProgramHeader* pHeader = (ElfProgramHeader*)(imgbuf + header->progHeaderOffset);
    uint64_t amountOfPages = 0;
    uint64_t minVAddr = UINT64_MAX;
    uint64_t maxVAddr = 0;
    for(int i = 0; i < header->progHeaderTableCount; i++){
        if(pHeader->programType == PROGHEAD_LOAD){
            if(minVAddr > pHeader->programVAddr) minVAddr = pHeader->programVAddr;
            if(maxVAddr < pHeader->programVAddr + pHeader->programMemorySize) maxVAddr = pHeader->programVAddr + pHeader->programMemorySize;
        }
        pHeader++;
    }
    amountOfPages = EFI_SIZE_TO_PAGES(maxVAddr - minVAddr);
    EFI_PHYSICAL_ADDRESS address = 0x0; // temp address
    EFI_STATUS status = qol_return_systab()->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderCode, amountOfPages, &address);
    if(status != EFI_SUCCESS){
        qol_halt_system("Not enough memory for nearlands kernel");
    }
    return address - minVAddr;
}

static inline void apply_reloc(char* imgbuf, ElfHeader* header, uint64_t baseAddr){
    ElfSecHeader* sHeader = (ElfSecHeader*)(imgbuf + header->secHeaderOffset);
    for(int i = 0; i < header->secHeaderTableCount; i++){
        if(sHeader->secType != SECHEAD_RELOCATE_WITH_ADDENDS) goto SKIP;

        ElfRelocation* relaEntries = (ElfRelocation*)(imgbuf + sHeader->secOffset);
        int eCount = sHeader->secSize / sHeader->secEntrySize;

        for(int j = 0; j < eCount; j++){
            uint64_t* target = (uint64_t*)(baseAddr + relaEntries[j].offset);
            *target = baseAddr + relaEntries[j].addend;
        }
        SKIP:
        sHeader++;
    }
}


void elfldr_load_image(Config conf, int mode, EFI_HANDLE image){
    qol_printf("Loading %s\n", conf.kernel);
    wchar buf[512];
    atow(conf.kernel, buf, 512);
    EFI_FILE_PROTOCOL* file = fs_open_file(buf, EFI_FILE_MODE_READ);
    if(file == NULL){
        qol_halt_system("elfldr failed: couldn't find image");
    }
    char* imgbuf = fs_read_file(file);
    if(imgbuf == NULL){
        qol_halt_system("elfldr failed: couldn't read executable");
    }
    ElfHeader* header = (ElfHeader*)imgbuf;
    if(memcmp(ELF_MAGIC, header->magic, 4) != 0){
        qol_halt_system("elfldr failed: kernel is not an ELF executable");
    }
    qol_printf("kernel is an ELF executable!\n");
    //qol_printf("unimplemented (yet), please reboot");
    ElfProgramHeader* pHeader = (ElfProgramHeader*)(imgbuf + header->progHeaderOffset);
    ElfSecHeader* sHeader = (ElfSecHeader*)(imgbuf + header->secHeaderOffset);
    char* loc = (imgbuf + header->secHeaderStrTableIndex);
    uint64_t paddress = address_to_allocate_to(imgbuf, header);
    uint8_t firstSeg = 0;
    uint64_t base = 0;
    for(int i = 0; i < header->progHeaderTableCount; i++){
        if(pHeader->programType == PROGHEAD_LOAD){
            uint64_t pages = EFI_SIZE_TO_PAGES(pHeader->programMemorySize);
            EFI_PHYSICAL_ADDRESS address = 0;
            
            address = paddress + pHeader->programVAddr;
            
            qol_printf("supposed to be put at 0x%x\n", pHeader->programVAddr);
            qol_printf("at %d: %d pages 0x%x\n", i, pages, address);
            memset((void*)address, 0, pHeader->programMemorySize);
            memcpy((void*)address, (void*)(imgbuf + pHeader->programOffset), pHeader->programFileSize);
        }
        pHeader++;
    }
    apply_reloc(imgbuf, header, paddress);


    UINTN memMapSize;
    EFI_MEMORY_DESCRIPTOR* efiMemMap = NULL;
    UINTN mapKey = 0;
    UINTN descriptorSize = 0;
    UINT32 descriptorVersion = 0;
    qol_return_systab()->BootServices->GetMemoryMap(&memMapSize, efiMemMap, &mapKey, &descriptorSize, &descriptorVersion);
    memMapSize += descriptorSize * 8;
    EFI_STATUS status = qol_return_systab()->BootServices->AllocatePool(EfiLoaderData, memMapSize, (void**)&efiMemMap);
    if(status != EFI_SUCCESS){
        qol_halt_system("Couldn't allocate memory for efi memmap");
    }
    status = qol_return_systab()->BootServices->GetMemoryMap(&memMapSize, efiMemMap, &mapKey, &descriptorSize, &descriptorVersion);
    if(status != EFI_SUCCESS){
        qol_halt_system("Couldn't get memory map");
    }
    uint64_t memsize = 0;
    uint64_t entries = memMapSize / descriptorSize;
    MemoryMap memmap = {NULL, 0};
    qol_return_systab()->BootServices->AllocatePool(EfiLoaderData, entries * sizeof(MemoryMapEntry), (void**)&memmap.memEntries);
    if(status != EFI_SUCCESS){
        qol_halt_system("Couldn't allocate memory for memmap");
    }
    memmap.amount = entries;
    for(int i = 0; i < entries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint8_t*)efiMemMap + (i * descriptorSize));

        memmap.memEntries[i].base = desc->PhysicalStart;
        memmap.memEntries[i].size = desc->NumberOfPages * 4096;
        memmap.memEntries[i].types = desc->Type;
        memsize += memmap.memEntries[i].size;
    }

    for(int i = 0; i < header->secHeaderTableCount; i++){
        qol_printf("%s\n", get_elf_section_name(imgbuf, header, sHeader));
        if(strcmp(".ldr", get_elf_section_name(imgbuf, header, sHeader)) == 0){
            LoaderInfo info = {
                memsize,
                1,
                memmap,
                {graphics_return_gop_info().fbAddress,
                        graphics_return_gop_info().fbSize, graphics_return_gop_info().width,
                        graphics_return_gop_info().height, graphics_return_gop_info().pixelPerScanLine}
            };
            uint64_t addr = paddress + sHeader->secVAddr;
            memcpy((void*)addr, &info, sizeof(LoaderInfo));
        }
        sHeader++;
    }
    void (*kernel_entry)(void) = (void(*)())(header->entrypoint + paddress);
    fs_close_file(file);
    qol_return_systab()->BootServices->FreePool(conf.kernel);
    qol_return_systab()->BootServices->FreePool(conf.name);
    qol_return_systab()->BootServices->FreePool(imgbuf);
    qol_return_systab()->BootServices->ExitBootServices(imgbuf, mapKey);
    graphics_clear(RGB(0, 0, 0));
    qol_return_systab()->ConOut->Reset(qol_return_systab()->ConOut, TRUE);
    // bye bye! - NearLDR
    kernel_entry();
    



    while(1){continue;}
}