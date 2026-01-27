#include <ldr/abs.h>
#include <ldr/graphics.h>
#include <ldr/panic.h>
#include <mm/mem.h>
#include <ar/serial.h>
#include <EFI/UefiBaseType.h>
#include <EFI/UefiSpec.h>
#include <fs/sfs.h>
#include <ldr/gop.h>
#include <ldr/peldr.h>
#include <ldr/term.h>
#include <types.h>









i32 LdrEfiEntry(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable){
    LdrSetupAbstractions(imageHandle, systemTable);
    ArPrintToSerial(QSTR("hello!\n"));
    boolean result = LdrSetupGOP();
    if(result == FALSE) ArPrintToSerial(QSTR("Failure to setup Graphics Output Protocol, going off with serial mode"));
    ArPrintToSerial(QSTR("set up the graphics output protocol\n"));
    result = MmInitPaging();
    if(result == FALSE) LdrPanic(QSTR("Failure to setup Paging"));
    ArPrintToSerial(QSTR("set up paging\n"));
    result = SFsSetupSimpleFilesystem();
    if(result == FALSE) LdrPanic(QSTR("Failure to setup Simple File System"));
    ArPrintToSerial(QSTR("set up simple file system\n"));

    LdrTermClear();
    
    LdrTermPrint(TERM_STATUS_PASS, QSTR("Nearmonia Bootloader\n"));
   
    LdrPeLoadPEImageAsKernel("\\SYSTEM\\narnify.sys");
    while(1){continue;}
    return 0;
}