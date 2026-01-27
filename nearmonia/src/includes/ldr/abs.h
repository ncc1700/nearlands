#ifndef ABS_H_INCLUDED
#define ABS_H_INCLUDED








#include <types.h>


#include <EFI/Uefi.h>
#include <EFI/UefiBaseType.h>



void LdrSetupAbstractions(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systab);
EFI_SYSTEM_TABLE* LdrReturnSystemTable();
EFI_HANDLE LdrReturnImagehandle();










#endif