#ifndef QOL_H_INCLUDED
#define QOL_H_INCLUDED







#include "externheaders/efi/Uefi.h"




void qol_initalize_abstractions(EFI_SYSTEM_TABLE* SystemTable);
EFI_SYSTEM_TABLE* qol_return_systab();
void qol_puts(wchar* string);
void qol_halt_system(wchar* error);


#endif