#include "arch/includes/mem.h"
#include "arch/includes/serial.h"
#include "extern/EFI/UefiBaseType.h"
#include "extern/EFI/UefiSpec.h"
#include "fs.h"
#include "gop.h"
#include "peldr.h"
#include "qol.h"
#include "term.h"
#include "types.h"









i32 LdrEfiEntry(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable){
    QolSetupQolAbstractions(imageHandle, systemTable);
    QolSerialFormatPrint("hello!\n");
    boolean result = LdrSetupGOP();
    if(result == FALSE) LdrArPrintToSerial("Failure to setup Graphics Output Protocol, going off with serial mode");
    QolSerialFormatPrint("set up the graphics output protocol\n");
    result = LdrMmInitPaging();
    if(result == FALSE) QolPanic("Failure to setup Paging");
    QolSerialFormatPrint("set up paging\n");
    result = LdrFsSetupFilesystem();
    if(result == FALSE) QolPanic("Failure to setup Simple File System");
    QolSerialFormatPrint("set up simple file system\n");

    TermClear();
    
    TermPrint(TERM_STATUS_PASS, "Nearmonia Bootloader Init PASS\n");

   
    LdrPeLoadPEImageAsKernel("\\SYSTEM\\narnify.sys");
    while(1){continue;}
    return 0;
}