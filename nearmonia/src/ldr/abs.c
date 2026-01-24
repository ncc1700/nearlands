#include "abs.h"







static EFI_SYSTEM_TABLE* globalSysTab = NULL;
static EFI_HANDLE globalHandle = NULL;



void _fltused(){}

void LdrSetupAbstractions(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systab){
    globalHandle = handle;
    globalSysTab = systab;
}

EFI_SYSTEM_TABLE* LdrReturnSystemTable(){
    return globalSysTab;
}

EFI_HANDLE LdrReturnImagehandle(){
    return globalHandle;
}