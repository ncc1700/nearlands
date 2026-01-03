#include "arch/includes/mem.h"
#include "extern/EFI/UefiBaseType.h"
#include "extern/EFI/UefiSpec.h"
#include "gop.h"
#include "qol.h"
#include "term.h"
#include "types.h"









i32 LdrEfiEntry(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable){
    QolSetupQolAbstractions(imageHandle, systemTable);
    LdrSetupGOP();
    LdrMmInitPaging();
    systemTable->ConOut->ClearScreen(systemTable->ConOut);
    char buffer[10] = {0};
    MemoryMap* memMap = LdrMmRetrieveCurrentMemoryMap(FALSE);
    u64 addr = 0x4B9999;
    EFI_STATUS status = QolReturnSystemTable()->BootServices->AllocatePages(AllocateAnyPages, 
                                                                                EfiLoaderCode, 4, &addr);
    if(status != EFI_SUCCESS){
        QolUefiFormatPrint("H\n");
        while(1){continue;}
    }
    LdrMmMapHigherHalfMemoryForKernel(addr);
    TermPrint(TERM_STATUS_PASS, "Mapped kernel memory at 0x%llu + HHDM offset\n", HHDM_OFFSET);
    u64* test_ptr = (u64*)HHDM_OFFSET;
    *test_ptr = 0x1; 
    TermPrint(TERM_STATUS_PASS, "Was able to map and write to HHDM address!");
    if(memMap == NULL){
        TermPrint(TERM_STATUS_ERROR, "Couldn't access memory map!");
    } else TermPrint(TERM_STATUS_PASS, "Retrieved memory map! Size of all memory is %lu, memory map amount %d\n", 
                            memMap->sizeOfEntireMemory, memMap->amountOfEntries);
    while(1){continue;}
    return 0;
}