#include "arch/includes/mem.h"
#include "gop.h"
#include "qol.h"
#include "term.h"
#include "types.h"









i32 LdrEfiEntry(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable){
    systemTable->ConOut->ClearScreen(systemTable->ConOut);
    QolSetupQolAbstractions(imageHandle, systemTable);
    LdrSetupGOP();
    

    MemoryMap* memMap = LdrMmRetrieveCurrentMemoryMap();
    if(memMap == NULL){
        TermPrint(TERM_STATUS_ERROR, "Couldn't access memory map!");
    } else TermPrint(TERM_STATUS_PASS, "Retrieved memory map! Size of all memory is %lu, memory map amount %d\n", 
                            memMap->sizeOfEntireMemory, memMap->amountOfEntries);
    while(1){continue;}
    return 0;
}