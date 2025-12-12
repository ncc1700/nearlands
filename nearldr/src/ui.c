#include "ui.h"
#include "externheaders/efi/SimpleTextIn.h"
#include "externheaders/efi/UefiBaseType.h"
#include "externheaders/efi/UefiSpec.h"
#include "graphics.h"
#include "peldr.h"
#include "qol.h"
#include <stdint.h>
#include "elfldr.h"

#define FG_COLOR RGB(210, 210, 210)
#define BG_COLOR RGB(0, 0, 130)


void render_ui(Config conf, EFI_HANDLE image){
    uint8_t exit = 0;
    UINTN index = 0;
    int64_t add = -80;
    EFI_INPUT_KEY key;
    graphics_clear(BG_COLOR);
    //graphics_draw_rect(0, 0, graphics_return_gop_info().width, 30, RGB(180, 180, 180));
    graphics_draw_rect(0,
                        graphics_return_gop_info().height - 30,
                        graphics_return_gop_info().width, 30, FG_COLOR);

    char* headertext = "Nearlands Boot Manager";
    size_t headertextwidth = graphics_measure_text(headertext, 1);
    graphics_print(headertext, 10,
                    30, 1, FG_COLOR);
    graphics_draw_rect(10, 40, headertextwidth + 10, 1, FG_COLOR);
    graphics_draw_rect(10, 45, headertextwidth + 10, 1, FG_COLOR);

    char* entertext = "W to go up | S to go down | Space to Enter";
    graphics_print(entertext, 10,
                    graphics_return_gop_info().height - 16,
                    1, BG_COLOR);


    graphics_print("Boot Normally", 30, graphics_return_gop_info().height / 2 - 120,
                1, FG_COLOR);
    graphics_print("Boot in Debug Mode (USES ELFLDR)", 30, graphics_return_gop_info().height / 2 - 80,
                1, FG_COLOR);
    graphics_print("Reboot", 30,
                        graphics_return_gop_info().height / 2 - 40, 1, FG_COLOR);
    graphics_print("Shutdown", 30,
                        graphics_return_gop_info().height / 2, 1, FG_COLOR);
    graphics_print("Return to UEFI", 30,
                        graphics_return_gop_info().height / 2 + 40, 1, FG_COLOR);
    while(exit == 0){
        if(conf.pause == 0){
            break;
        }
        graphics_print("<-", graphics_return_gop_info().width - 40,
                            graphics_return_gop_info().height / 2 + add, 1, FG_COLOR);
        qol_return_systab()->BootServices->WaitForEvent(1, &qol_return_systab()->ConIn->WaitForKey, &index);
        qol_return_systab()->ConIn->ReadKeyStroke(qol_return_systab()->ConIn, &key);
        if(key.UnicodeChar == L'w'){
            if(add <= -120){
                continue;
            }
            graphics_print("<-", graphics_return_gop_info().width - 40,
                            graphics_return_gop_info().height / 2 + add, 1, BG_COLOR);
            add -= 40;
        }
        if(key.UnicodeChar == L's'){
            if(add >= 40){
                continue;
            }
            graphics_print("<-", graphics_return_gop_info().width - 40,
                            graphics_return_gop_info().height / 2 + add, 1, BG_COLOR);
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
            peldr_load_image(conf, 1, image);
            //qol_halt_system("unimplemented");
            break;
        }
        case -80:{
            elfldr_load_image(conf, 1, image);
            //qol_halt_system("unimplemented");
            break;
        }
        case -40:{
            qol_printf("Rebooting");
            EFI_RESET_TYPE h = 1;
            qol_return_systab()->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
            break;
        }
        case 0:{
            qol_printf("Shutting Down");
            EFI_RESET_TYPE h = 1;
            qol_return_systab()->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
            break;
        }
        case 40:{
            qol_printf("returning to UEFI");
            break;
        }
        default:{
            qol_halt_system("Invalid option chosen, please report this");
        }
    }
}
