#include "term.h"
#include "extern/nanoprintf/nprintfimpl.h"
#include "graphics.h"
#include "types.h"
#include <stdarg.h>


static u32 termCurrentX = 10;
static u32 termCurrentY = 10;



void TermClear(){
    GraphicsDrawRect(0, 0, GraphicsReturnData()->width, GraphicsReturnData()->height, 0x000000);
    termCurrentX = 10;
    termCurrentY = 10;
}




void TermPuts(boolean addNewLine, const char* string, u64 color){
    // char buffer[128];
    // va_list arg;
    // va_start(arg, string);
    // impl_vsnprintf(buffer, 128, string, arg);
    // va_end(arg);
    GraphicsDrawString(string, termCurrentX, termCurrentY, 1, color);
    if(addNewLine == TRUE){
        termCurrentX = 10;
        termCurrentY += 12;
        if(termCurrentY >= GraphicsReturnData()->height){
            TermClear();
        }
    } else {
        termCurrentX += GraphicsMeasureTextSizeFromDefaultFont(string, 1);
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
            TermPuts(FALSE, "[?] ", 0xFFFFFF);
            TermPuts(TRUE, buffer, 0xFFFFFF);
            break;
    }
}