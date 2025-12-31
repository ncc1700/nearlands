#include "gop.h"
#include "graphics.h"
#include "qol.h"
#include "term.h"
#include "types.h"









i32 LdrEfiEntry(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable){
    systemTable->ConOut->ClearScreen(systemTable->ConOut);
    QolSetupQolAbstractions(imageHandle, systemTable);
    LdrSetupGOP();
    

    //GraphicsDrawRect(0, 0, 100, 50, 0xFFFFFF);
    //GraphicsDrawString("[+] Hello World!", 0, 0, 1, 0xFFFFFF);
    TermPrint(TERM_STATUS_ERROR, "This is an error %d", 1);
    TermPrint(TERM_STATUS_PASS, "This is a pass %d", 1);
    TermPrint(TERM_STATUS_INFO, "This is an info %d", 1);
    TermPrint(TERM_STATUS_WARNING, "This is a warning %d", 1);
    TermPrint(TERM_STATUS_UNKNOWN, "This is unknown %d", 1);
    while(1){continue;}
    return 0;
}