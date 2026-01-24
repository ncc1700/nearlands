#include "term.h"
#include "../ar/includes/serial.h"
#include "../extern/nanoprintf/nprintfimpl.h"
#include "graphics.h"
#include "../types.h"
#include <stdarg.h>


static u32 termCurrentX = 10;
static u32 termCurrentY = 10;


void LdrTermClear(){
    // ONLY IN UEFI
    if(LdrGraphicsReturnData()->init == 1){
        LdrGraphicsDrawRect(0, 0, LdrGraphicsReturnData()->width, 
                            LdrGraphicsReturnData()->height, 0x112121);
    }
    termCurrentX = 10;
    termCurrentY = 10;
    ArSerialWrite('\n');
}


void LdrTermMoveDown(u32 amount){
    for(u32 i = 0; i < amount; i++){
        termCurrentX = 10;
        termCurrentY += 12;
        if(termCurrentY >= LdrGraphicsReturnData()->height){
            LdrTermClear();
        }
    }
}



void LdrTermPuts(boolean addNewLine, QString str, u64 color){
    if(LdrGraphicsReturnData()->init == 1){
        LdrGraphicsDrawString(str, termCurrentX, termCurrentY, 1, color);
    }
    ArPrintToSerial(str);
    if(addNewLine == TRUE){
        termCurrentX = 10;
        termCurrentY += 12;
        if(termCurrentY >= LdrGraphicsReturnData()->height){
            LdrTermClear();
        }
        ArSerialWrite('\n');
    } else {
        if(LdrGraphicsReturnData()->init == 1){
            termCurrentX += LdrGraphicsMeasureTextSizeFromDefaultFont(C(str.buffer), 1);
        }
    }
}



void LdrTermPrint(Status stat, QString str, ...){
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
            LdrTermPuts(FALSE, QSTR("[-] "), 0xFF0000);
            LdrTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        case TERM_STATUS_PASS:
            LdrTermPuts(FALSE, QSTR("[+] "), 0x00FF00);
            LdrTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        case TERM_STATUS_INFO:
            LdrTermPuts(FALSE, QSTR("[.] "), 0x00AAFF);
            LdrTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        case TERM_STATUS_WARNING:
            LdrTermPuts(FALSE, QSTR("[!] "), 0xFFFF00);
            LdrTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        case TERM_STATUS_IMPINFO:
            LdrTermPuts(FALSE, QSTR("[!] "), 0xFDA172);
            LdrTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
        default:
            LdrTermPuts(TRUE, QSTR(buffer), 0xFFFFFF);
            break;
    }
}