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

// stolen from POSIX-UEFIs example

#define ELFMAG      "\177ELF"
#define SELFMAG     4
#define EI_CLASS    4       /* File class byte index */
#define ELFCLASS64  2       /* 64-bit objects */
#define EI_DATA     5       /* Data encoding byte index */
#define ELFDATA2LSB 1       /* 2's complement, little endian */
#define ET_EXEC     2       /* Executable file */
#define PT_LOAD     1       /* Loadable program segment */
#ifdef __x86_64__
#define EM_MACH     62      /* AMD x86-64 architecture */
#endif
#ifdef __aarch64__
#define EM_MACH     183     /* ARM aarch64 architecture */
#endif

typedef struct
{
    uint8_t  e_ident[16];   /* Magic number and other info */
    uint16_t e_type;        /* Object file type */
    uint16_t e_machine;     /* Architecture */
    uint32_t e_version;     /* Object file version */
    uint64_t e_entry;       /* Entry point virtual address */
    uint64_t e_phoff;       /* Program header table file offset */
    uint64_t e_shoff;       /* Section header table file offset */
    uint32_t e_flags;       /* Processor-specific flags */
    uint16_t e_ehsize;      /* ELF header size in bytes */
    uint16_t e_phentsize;   /* Program header table entry size */
    uint16_t e_phnum;       /* Program header table entry count */
    uint16_t e_shentsize;   /* Section header table entry size */
    uint16_t e_shnum;       /* Section header table entry count */
    uint16_t e_shstrndx;    /* Section header string table index */
} Elf64_Ehdr;

typedef struct
{
    uint32_t p_type;        /* Segment type */
    uint32_t p_flags;       /* Segment flags */
    uint64_t p_offset;      /* Segment file offset */
    uint64_t p_vaddr;       /* Segment virtual address */
    uint64_t p_paddr;       /* Segment physical address */
    uint64_t p_filesz;      /* Segment size in file */
    uint64_t p_memsz;       /* Segment size in memory */
    uint64_t p_align;       /* Segment alignment */
} Elf64_Phdr;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static inline void relocate_exec(){

}
void elfldr_load_image(Config conf, int mode, EFI_HANDLE image){
    wchar buf[512];
    atow(conf.kernel, buf, 512);
    EFI_FILE_PROTOCOL* file = fs_open_file(buf, EFI_FILE_MODE_READ);
    if(file == NULL){
        qol_halt_system(L"Couldn't find image");
    }
    char* elfbuf = fs_read_file(file);
    if(elfbuf == NULL){
        qol_halt_system(L"error");
    }
    Elf64_Ehdr* elf = (Elf64_Ehdr*)elfbuf;
    if(memcmp(elf->e_ident, ELFMAG, SELFMAG) != 0){
        qol_halt_system(L"not a valid ELF executable!");
    }
    if(elf->e_ident[EI_CLASS] != ELFCLASS64){
        qol_halt_system(L"kernel needs to be 64bits");
    }
    if(elf->e_type != ET_EXEC){
        qol_halt_system(L"kernel not an executable");
    }
    if(elf->e_phnum < 0){
        qol_halt_system(L"elf->e_phnum < 0");
    }

    uint64_t imageSize = 0;
    for(uint64_t i = 0; i < elf->e_phnum; i++){
        Elf64_Phdr* phdr = (Elf64_Phdr*)(elfbuf + elf->e_phoff + i * elf->e_phentsize);
        if(phdr->p_type == PT_LOAD){
            imageSize = MAX(imageSize, phdr->p_vaddr + phdr->p_memsz);
        }
    }
    uint64_t amountOfPages = (imageSize + 0xFFF) / 0x1000;
    uint64_t image_base = 0;
    EFI_STATUS status = qol_return_systab()->BootServices->AllocatePages(AllocateAnyPages,
                                                                         EfiLoaderCode, amountOfPages,
                                                                    (EFI_PHYSICAL_ADDRESS*)&image_base);
    if (status != EFI_SUCCESS) {
        qol_halt_system(L"Not enough memory to load kernel");
    }
    int i = 0;
    Elf64_Phdr* phdr;
    for(phdr = (Elf64_Phdr*)(elfbuf + elf->e_phoff), i = 0;
        i < elf->e_phnum;
        i++, phdr = (Elf64_Phdr *)((uint8_t *)phdr + elf->e_phentsize))
    {
        if(phdr->p_type == PT_LOAD){
            memcpy((void*)phdr->p_vaddr, elfbuf + phdr->p_offset, phdr->p_filesz);
            memset((void*)(phdr->p_vaddr + phdr->p_filesz),
                    0,  phdr->p_memsz - phdr->p_filesz);
        }
    }
    if(image_base != elf->e_entry){
        relocate_exec();
    }
    uint64_t entry = elf->e_entry;
    fs_close_file(file);
    qol_return_systab()->BootServices->FreePool(elfbuf);
    void (*EntryPoint)() = (void(*)())(entry);
    EntryPoint();

}
