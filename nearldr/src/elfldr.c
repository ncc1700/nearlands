#include "elfldr.h"
#include "externheaders/efi/SimpleFileSystem.h"
#include "fs.h"
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
    qol_printf("%s\n", loc);
    qol_printf("%d\n", header->progHeaderTableCount);

    for(int i = 0; i < header->progHeaderTableCount; i++){
        if(pHeader->programType == PROGHEAD_LOAD){
            //qol_printf("Program header in 0x%x\n", pHeader->programPAddr);
            
        }
        pHeader++;
    }

    for(int i = 0; i < header->secHeaderTableCount; i++){
        if(sHeader->secType == PROGHEAD_LOAD){
            //qol_printf("%s\n", get_elf_section_name(imgbuf, header, sHeader));
        }
        sHeader++;
    }



    fs_close_file(file);
    while(1){continue;}
}