#include "qol.h"
#include "arch/arch.h"
#include "externheaders/efi/UefiSpec.h"
#include "graphics.h"
#include <stddef.h>
#include <stdint.h>




static EFI_SYSTEM_TABLE* systab = NULL;




void qol_initalize_abstractions(EFI_SYSTEM_TABLE* SystemTable){
    systab = SystemTable;
}

EFI_SYSTEM_TABLE* qol_return_systab(){
    return systab;
}

void qol_puts(wchar* string){
    qol_return_systab()->ConOut->OutputString(systab->ConOut, (CHAR16*)string);
    qol_return_systab()->ConOut->OutputString(systab->ConOut, (CHAR16*)L"\n");
}


void qol_halt_system(wchar* error){
    if(graphics_get_if_init() == 1){
        graphics_clear(RGB(0, 255, 255));
        graphics_print("An error has occured and NearLDR has been halted", 
            10, 10, 2, RGB(0, 0, 0));
    }
    qol_puts(error);
    halt();
}



void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    uint8_t *restrict pdest = (uint8_t *restrict)dest;
    const uint8_t *restrict psrc = (const uint8_t *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

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
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}