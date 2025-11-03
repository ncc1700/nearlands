#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED







#include "../uefi/uefi.h"


typedef struct _GopInfo {
    uint64_t fbAddress;
    uint64_t fbSize;
    uint32_t width;
    uint32_t height;
    uint32_t pixelPerScanLine;
} GopInfo;


uint32_t RGB(uint8_t R, uint8_t G, uint8_t B);
void graphics_get_gop_info();
void graphics_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void graphics_test_graphics();
void graphics_clear();
GopInfo graphics_return_gop_info();
void graphics_putchar(char c, uint32_t x, uint32_t y, uint32_t size, uint32_t color);
void graphics_print(char* s, uint32_t x, uint32_t y, uint32_t size, uint32_t color);
#endif