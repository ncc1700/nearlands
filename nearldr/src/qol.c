#include "qol.h"
#include "arch/arch.h"
#include "externheaders/efi/UefiSpec.h"
#include "graphics.h"
#include <stddef.h>
#include <stdint.h>
#include "externheaders/mini-printf/mini-printf.h"
#include "string.h"



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

void qol_printf(char* string, ...){
    char s[1024];
    va_list args;
    va_start(args, string);
    mini_vsnprintf(s, sizeof(s), string, args);
    va_end(args);

    wchar ws[1024];
    atow(s, ws, 1024);
    qol_puts(ws);
    //qol_return_systab()->ConOut->SetMode()
}


void qol_halt_system(char* error){
    if(graphics_get_if_init() == 1){
        graphics_clear(RGB(0, 255, 255));
        graphics_print("An error has occured and NearLDR has been halted", 
            10, 10, 2, RGB(0, 0, 0));
    }
    qol_printf(error);
    halt();
}



