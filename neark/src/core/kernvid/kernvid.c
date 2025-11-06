#include "kernvid.h"
#include <stdint.h>
#include <stddef.h>
#include "../../externheaders/font8x8/font8x8_basic.h"
#include "../../ldrconfig.h"

static KernVid info = {0};


uint32_t RGB(uint8_t R, uint8_t G, uint8_t B){
    return (R << 16) | (G << 8) | (B);
}

uint8_t kernvid_get_if_init(){
    return info.init;
}

void kernvid_initialize(Framebuffer fb){
    info.fbAddress = fb.fbAddress;
    info.fbSize = fb.fbSize;
    info.height = fb.height;
    info.width = fb.width;
    info.pixelPerScanLine = fb.pixelPerScanLine;
    info.init = 1;
}

void kernvid_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color){
    volatile uint32_t* fb = (volatile uint32_t*)info.fbAddress;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fb[(y + i) * info.pixelPerScanLine + (x + j)] = color;
        }
    }
}


void kernvid_test_graphics(){
    volatile uint32_t* fb = (volatile uint32_t*)info.fbAddress;
    for(int i = 0; i < info.width; i++){
        for(int j = 0; j < info.height; j++){
            fb[j * info.pixelPerScanLine + i] = RGB(j + i, j - i, i * j);
        }
    }
}

void kernvid_clear(uint32_t color){
    volatile uint32_t* fb = (volatile uint32_t*)info.fbAddress;
    for(int i = 0; i < info.width; i++){
        for(int j = 0; j < info.height; j++){
            fb[j * info.pixelPerScanLine + i] = color;
        }
    }
}


// use this instead, PLEASE do not do "extern KernVid info"
KernVid kernvid_return_info(){
    return info;
}

void kernvid_putchar(char c, uint32_t x, uint32_t y, uint32_t size, uint32_t color){
    if (c < 32 || c > 126) {
        return;
    }
    const uint8_t *bitmap = font8x8_basic[c - 32];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (bitmap[i] & (1 << j)) {
                kernvid_draw_rect(x + j * size, y + i * size, size, size, color);
            }
        }
    }
}


void kernvid_print(char* s, uint32_t x, uint32_t y, uint32_t size, uint32_t color){
    uint32_t current_x = x;
    uint32_t current_y = y;

    while (*s != '\0') {
        if (*s == '\n') {
            current_y += size * 10;
            current_x = x;
        } else {
            kernvid_putchar(*s, current_x, current_y, size, color);
            current_x += size * 8;
        }
        s++;
    }
}

size_t kernvid_measure_text(char* s, uint32_t fontsize){
    size_t size = 0;
    while(*s != '\0'){
        size += fontsize * 8;
        s++;
    }
    return size;
}
