#include "term.h"
#include "../ar/includes/serial.h"
#include "../extern/nanoprintf/nprintfimpl.h"
#include "graphics.h"
#include "../types.h"
#include <stdarg.h>


static u32 termCurrentX = 10;
static u32 termCurrentY = 10;


void KeTermClear(){
    // ONLY IN UEFI
    if(KeGraphicsReturnData()->init == 1 && KeGraphicsReturnData()->owners == OWNER_DEBUG_TERM){
        KeGraphicsDrawRect(0, 0, KeGraphicsReturnData()->width, 
                            KeGraphicsReturnData()->height, 0x112121);
    }
    termCurrentX = 10;
    termCurrentY = 10;
    ArSerialWrite('\n');
}


void KeTermMoveDown(u32 amount){
    for(u32 i = 0; i < amount; i++){
        termCurrentX = 10;
        termCurrentY += 12;
        if(termCurrentY >= KeGraphicsReturnData()->height){
            KeTermClear();
        }
    }
}



void KeTermPuts(boolean addNewLine, const char* string, u64 color){
    // char buffer[128];
    // va_list arg;
    // va_start(arg, string);
    // impl_vsnprintf(buffer, 128, string, arg);
    // va_end(arg);
    if(KeGraphicsReturnData()->init == 1 && KeGraphicsReturnData()->owners == OWNER_DEBUG_TERM){
        KeGraphicsDrawString(string, termCurrentX, termCurrentY, 1, color);
    }
    ArPrintToSerial(string);
    if(addNewLine == TRUE){
        termCurrentX = 10;
        termCurrentY += 12;
        if(termCurrentY >= KeGraphicsReturnData()->height){
            KeTermClear();
        }
        ArSerialWrite('\n');
    } else {
        if(KeGraphicsReturnData()->init == 1 && KeGraphicsReturnData()->owners == OWNER_DEBUG_TERM){
            termCurrentX += KeGraphicsMeasureTextSizeFromDefaultFont(string, 1);
        }
    }
}

void KeTermPrint(Status stat, char* string, ...){
    char buffer[128];
    va_list arg;
    va_start(arg, string);
    impl_vsnprintf(buffer, 128, string, arg);
    va_end(arg);
    switch(stat){
        case TERM_STATUS_ERROR:
            KeTermPuts(FALSE, "[-] ", 0xFF0000);
            KeTermPuts(TRUE, buffer, 0xFFFFFF);
            break;
        case TERM_STATUS_PASS:
            KeTermPuts(FALSE, "[+] ", 0x00FF00);
            KeTermPuts(TRUE, buffer, 0xFFFFFF);
            break;
        case TERM_STATUS_INFO:
            KeTermPuts(FALSE, "[.] ", 0x00AAFF);
            KeTermPuts(TRUE, buffer, 0xFFFFFF);
            break;
        case TERM_STATUS_WARNING:
            KeTermPuts(FALSE, "[!] ", 0xFFFF00);
            KeTermPuts(TRUE, buffer, 0xFFFFFF);
            break;
        default:
            KeTermPuts(TRUE, buffer, 0xFFFFFF);
            break;
    }
}