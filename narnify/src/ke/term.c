#include "nrstatus.h"
#include <ke/term.h>
#include <ar/serial.h>
#include <nanoprintf/nprintfimpl.h>
#include <ke/graphics.h>
#include <types.h>
#include <stdarg.h>


static u32 termCurrentX = 10;
static u32 termCurrentY = 10;
//static u64 defaultColor = 0x112121;



static u64 colorPalette[] = {
    [PALETTE_BACKGROUND] = 0x112121,
    [PALETTE_TEXT] = 0xFFFFFF,
    [PALETTE_ERROR] = 0xFF0000,
    [PALETTE_PASS] = 0x00FF00,
    [PALETTE_WARNING] = 0xFFFF00,
    [PALETTE_INFO] = 0x00AAFF,
    [PALETTE_IMPINFO] = 0xFDA172
};

void KeTermClear(){
    // ONLY IN UEFI
    if(KeGraphicsReturnData()->init == 1 && KeGraphicsReturnData()->owners == OWNER_DEBUG_TERM){
        KeGraphicsDrawRect(0, 0, KeGraphicsReturnData()->width, 
                            KeGraphicsReturnData()->height, 
                            colorPalette[PALETTE_BACKGROUND]);
    }
    termCurrentX = 10;
    termCurrentY = 10;
    ArSerialWrite('\n');
}

void KeTermTakeOwnershipOfKernelGraphics(){
    KeGraphicsReturnData()->owners = OWNER_DEBUG_TERM;
}

NearStatus KeTermChangeSinglePaletteColor(ColorPalette palette, u64 color){
    u64 len = ARR_LENGTH(colorPalette);
    if(palette >= len){
        return STATUS_NOT_FOUND;
    }
    colorPalette[palette] = color;
    return STATUS_SUCCESS;
}

NearStatus KeTermChangePalette(u64* palette, u64 len){
    u64 palLen = ARR_LENGTH(colorPalette);
    if(len > palLen){
        return STATUS_TOO_LARGE;
    }
    for(u64 i = 0; i < len; i++){
        colorPalette[i] = palette[i];
    }
    return STATUS_SUCCESS;
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
            KeTermPuts(FALSE, QSTR("[-] "), colorPalette[PALETTE_ERROR]);
            KeTermPuts(TRUE, QSTR(buffer), colorPalette[PALETTE_TEXT]);
            break;
        case TERM_STATUS_PASS:
            KeTermPuts(FALSE, QSTR("[+] "), colorPalette[PALETTE_PASS]);
            KeTermPuts(TRUE, QSTR(buffer), colorPalette[PALETTE_TEXT]);
            break;
        case TERM_STATUS_INFO:
            KeTermPuts(FALSE, QSTR("[!] "), colorPalette[PALETTE_INFO]);
            KeTermPuts(TRUE, QSTR(buffer), colorPalette[PALETTE_TEXT]);
            break;
        case TERM_STATUS_WARNING:
            KeTermPuts(FALSE, QSTR("[!] "), colorPalette[PALETTE_WARNING]);
            KeTermPuts(TRUE, QSTR(buffer), colorPalette[PALETTE_TEXT]);
            break;
        case TERM_STATUS_IMPINFO:
            KeTermPuts(FALSE, QSTR("[!] "), colorPalette[PALETTE_IMPINFO]);
            KeTermPuts(TRUE, QSTR(buffer), colorPalette[PALETTE_TEXT]);
            break;
        default:
            KeTermPuts(TRUE, QSTR(buffer), colorPalette[PALETTE_TEXT]);
            break;
    }
}