#include "ui.h"
#include "externheaders/efi/SimpleTextIn.h"
#include "externheaders/efi/UefiBaseType.h"
#include "externheaders/efi/UefiSpec.h"
#include "graphics.h"
#include "peldr.h"
#include "qol.h"
#include <stdint.h>



void render_ui(Config conf, EFI_HANDLE image){
    uint8_t exit = 0;
    UINTN index = 0;
    int64_t add = -80;
    EFI_INPUT_KEY key;
    graphics_draw_rect(0, 0, graphics_return_gop_info().width, 30, RGB(180, 180, 180));
    graphics_draw_rect(0, 
                        graphics_return_gop_info().height - 30, 
                        graphics_return_gop_info().width, 30, RGB(180, 180, 180));

    char* headertext = "Nearlands Boot Manager";
    size_t headertextsize = graphics_measure_text(headertext, 1);
    graphics_print(headertext, (graphics_return_gop_info().width - headertextsize) / 2, 
                    14, 1, RGB(0, 0, 0));

    char* entertext = "Space=Choose    W=Go up the list    S=Go Down the list";
    graphics_print(entertext, 10, 
                    graphics_return_gop_info().height - 16, 
                    1, RGB(0, 0, 0));
    
    
    graphics_print("Boot Normally", 10, graphics_return_gop_info().height / 2 - 120, 
                1, RGB(255, 255, 255));
    graphics_print("Boot in Debug Mode", 10, graphics_return_gop_info().height / 2 - 80, 
                1, RGB(255, 255, 255));
    graphics_print("Reboot", 10, 
                        graphics_return_gop_info().height / 2 - 40, 1, RGB(255, 255, 255));
    graphics_print("Shutdown", 10, 
                        graphics_return_gop_info().height / 2, 1, RGB(255, 255, 255));
    graphics_print("Return to UEFI", 10, 
                        graphics_return_gop_info().height / 2 + 40, 1, RGB(255, 255, 255));
    while(exit == 0){
        if(conf.timeout == 0){
            break;
        }
        graphics_print("<-", graphics_return_gop_info().width - 40, 
                            graphics_return_gop_info().height / 2 + add, 1, RGB(255, 255, 255));
        qol_return_systab()->BootServices->WaitForEvent(1, &qol_return_systab()->ConIn->WaitForKey, &index);
        qol_return_systab()->ConIn->ReadKeyStroke(qol_return_systab()->ConIn, &key);
        if(key.UnicodeChar == L'w'){
            if(add <= -120){
                continue;
            }
            graphics_print("<-", graphics_return_gop_info().width - 40, 
                            graphics_return_gop_info().height / 2 + add, 1, RGB(0, 0, 0));
            add -= 40;
        }
        if(key.UnicodeChar == L's'){
            if(add >= 40){
                continue;
            }
            graphics_print("<-", graphics_return_gop_info().width - 40, 
                            graphics_return_gop_info().height / 2 + add, 1, RGB(0, 0, 0));
            add += 40;
        }
        if(key.UnicodeChar == ' '){
            exit = 1;
            continue;;
        }
    }
    graphics_clear(RGB(0, 0, 0));
    qol_return_systab()->ConOut->Reset(qol_return_systab()->ConOut, TRUE);
    switch(add){
        case -120:{
            peldr_load_image(conf, (LoaderConfiguration){0}, image);
            //qol_halt_system(L"unimplemented");
            break;
        }
        case -80:{
            peldr_load_image(conf, (LoaderConfiguration){1}, image);
            //qol_halt_system(L"unimplemented");
            break;
        }
        case -40:{
            qol_puts(L"Rebooting");
            EFI_RESET_TYPE h = 1;
            qol_return_systab()->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
            break;
        }
        case 0:{
            qol_puts(L"Shutting Down");
            EFI_RESET_TYPE h = 1;
            qol_return_systab()->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
            break;
        }
        case 40:{
            qol_puts(L"returning to UEFI");
            break;
        }
        default:{
            qol_halt_system(L"Invalid option chosen, please report this");
        }
    }
}


