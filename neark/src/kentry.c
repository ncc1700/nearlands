




#include "ldrconfig.h"
#include <stddef.h>
#include <stdint.h>

void __chkstk(){}

uint32_t RGB(uint8_t R, uint8_t G, uint8_t B){
    return (R << 16) | (G << 8) | (B);
}

void kernel_entry(LoaderInfo* info){
    volatile uint32_t* fb = (volatile uint32_t*)info->fb.fbAddress;
    for(int i = 0; i < info->fb.width; i++){
        for(int j = 0; j < info->fb.height; j++){
            fb[j * info->fb.pixelPerScanLine + i] = RGB(j + i, j - i, i * j);
        }
    }
    while(1){continue;}
}


