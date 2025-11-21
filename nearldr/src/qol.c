#include "qol.h"
#include "arch/arch.h"
#include "externheaders/efi/UefiSpec.h"
#include "graphics.h"
#include <stddef.h>
#include <stdint.h>




static EFI_SYSTEM_TABLE* systab = NULL;




void qol_initalize_abstractions(EFI_SYSTEM_TABLE* SystemTable){
    systab = SystemTable;
}

EFI_SYSTEM_TABLE* qol_return_systab(){
    return systab;
}

void qol_puts(wchar* string){
    qol_return_systab()->ConOut->OutputString(systab->ConOut, (CHAR16*)string);
    //qol_return_systab()->ConOut->OutputString(systab->ConOut, (CHAR16*)L"\n");
}


void qol_halt_system(wchar* error){
    if(graphics_get_if_init() == 1){
        graphics_clear(RGB(0, 255, 255));
        graphics_print("An error has occured and NearLDR has been halted", 
            10, 10, 2, RGB(0, 0, 0));
    }
    qol_puts(error);
    halt();
}



