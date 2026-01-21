#include "qol.h"
#include "arch/includes/serial.h"
#include "graphics.h"
#include "extern/nanoprintf/nprintfimpl.h"
#include "term.h"

void _fltused(){}

boolean QolInitSystem(){
    // STUB
    return FALSE;
}

void QolSerialFormatPrint(const char* string, ...){
    char buffer[128];
    va_list arg;
    va_start(arg, string);
    impl_vsnprintf(buffer, 128, string, arg);
    va_end(arg);
    ArPrintToSerial(string);
}

void QolPanic(const char* string){
    ArPrintToSerial("\n\n----------------------------------------------------------------\n");
    ArPrintToSerial("An error has occured!!!!\n\n");
    ArPrintToSerial(string);
    if(GraphicsReturnData()->init == 1){
        GraphicsDrawRect(0, 0, GraphicsReturnData()->width, GraphicsReturnData()->height, 0x0000FF);
        GraphicsDrawString("An Error Has Occured", 10, 10, 2, 0xFFFFFF);
        GraphicsDrawString(string, 10, 50, 1, 0xFFFFFF);
    }
    ArPrintToSerial("\n\n----------------------------------------------------------------\n");
    while(1){continue;}
}

/*
    Taken from Limines template as i believe my own impl
    would suck

    clang won't compile the code without these
*/

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    unsigned char *restrict pdest = (unsigned char *restrict)dest;
    const unsigned char *restrict psrc = (const unsigned char *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (unsigned char)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *pdest = (unsigned char *)dest;
    const unsigned char *psrc = (const unsigned char *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}