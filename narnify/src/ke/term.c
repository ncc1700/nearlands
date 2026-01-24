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



void KeTermPuts(boolean addNewLine, QString str, u64 color){
    if(KeGraphicsReturnData()->init == 1 && KeGraphicsReturnData()->owners == OWNER_DEBUG_TERM){
        KeGraphicsDrawString(str, termCurrentX, termCurrentY, 1, color);
    }
    ArPrintToSerial(str);
    if(addNewLine == TRUE){
        termCurrentX = 10;
        termCurrentY += 12;
        if(termCurrentY >= KeGraphicsReturnData()->height){
            KeTermClear();
        }
        ArSerialWrite('\n');
    } else {
        if(KeGraphicsReturnData()->init == 1 && KeGraphicsReturnData()->owners == OWNER_DEBUG_TERM){
            termCurrentX += KeGraphicsMeasureTextSizeFromDefaultFont(C(str.buffer), 1);
        }
    }
}



void KeTermPrint(Status stat, QString str, ...){
    char buffer[256];
    char cstr[128];

    u64 cindex = 0;
    for(cindex = 0; cindex < str.length; cindex++){
        if(cindex >= 127) break;
        cstr[cindex] = str.buffer[cindex];
    }
    cstr[cindex] = '\0';

    va_list arg;
    va_start(arg, C(str.buffer));
    impl_vsnprintf(buffer, 256, 
                    cstr, arg);
    va_end(arg);

    switch(stat){
        case TERM_STATUS_ERROR:
            KeTermPuts(FALSE, QSTR("[-] "), 0xFF0000);
            KeTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        case TERM_STATUS_PASS:
            KeTermPuts(FALSE, QSTR("[+] "), 0x00FF00);
            KeTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        case TERM_STATUS_INFO:
            KeTermPuts(FALSE, QSTR("[.] "), 0x00AAFF);
            KeTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        case TERM_STATUS_WARNING:
            KeTermPuts(FALSE, QSTR("[!] "), 0xFFFF00);
            KeTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        case TERM_STATUS_IMPINFO:
            KeTermPuts(FALSE, QSTR("[!] "), 0xFDA172);
            KeTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        default:
            KeTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
    }
}