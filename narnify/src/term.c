#include "term.h"
#include "arch/includes/serial.h"
#include "extern/nanoprintf/nprintfimpl.h"
#include "graphics.h"
#include "types.h"
#include <stdarg.h>


static u32 termCurrentX = 10;
static u32 termCurrentY = 10;


void TermClear(){
    // ONLY IN UEFI
    if(GraphicsReturnData()->init == 1 && GraphicsReturnData()->owners == OWNER_DEBUG_TERM){
        GraphicsDrawRect(0, 0, GraphicsReturnData()->width, GraphicsReturnData()->height, 0x000000);
    }
    termCurrentX = 10;
    termCurrentY = 10;
    ArEnterCharacterToSerial('\n');
}




void TermPuts(boolean addNewLine, const char* string, u64 color){
    // char buffer[128];
    // va_list arg;
    // va_start(arg, string);
    // impl_vsnprintf(buffer, 128, string, arg);
    // va_end(arg);
    if(GraphicsReturnData()->init == 1 && GraphicsReturnData()->owners == OWNER_DEBUG_TERM){
        GraphicsDrawString(string, termCurrentX, termCurrentY, 1, color);
    }
    ArPrintToSerial(string);
    if(addNewLine == TRUE){
        termCurrentX = 10;
        termCurrentY += 12;
        if(termCurrentY >= GraphicsReturnData()->height){
            TermClear();
        }
        ArEnterCharacterToSerial('\n');
    } else {
        if(GraphicsReturnData()->init == 1 && GraphicsReturnData()->owners == OWNER_DEBUG_TERM){
            termCurrentX += GraphicsMeasureTextSizeFromDefaultFont(string, 1);
        }
    }
}

void TermPrint(Status stat, char* string, ...){
    char buffer[128];
    va_list arg;
    va_start(arg, string);
    impl_vsnprintf(buffer, 128, string, arg);
    va_end(arg);
    switch(stat){
        case TERM_STATUS_ERROR:
            TermPuts(FALSE, "[-] ", 0xFF0000);
            TermPuts(TRUE, buffer, 0xFFFFFF);
            break;
        case TERM_STATUS_PASS:
            TermPuts(FALSE, "[+] ", 0x00FF00);
            TermPuts(TRUE, buffer, 0xFFFFFF);
            break;
        case TERM_STATUS_INFO:
            TermPuts(FALSE, "[.] ", 0x00AAFF);
            TermPuts(TRUE, buffer, 0xFFFFFF);
            break;
        case TERM_STATUS_WARNING:
            TermPuts(FALSE, "[!] ", 0xFFFF00);
            TermPuts(TRUE, buffer, 0xFFFFFF);
            break;
        default:
            TermPuts(TRUE, buffer, 0xFFFFFF);
            break;
    }
}