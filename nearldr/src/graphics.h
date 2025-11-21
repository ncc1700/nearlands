#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED







#include "externheaders/efi/Uefi.h"
#include <stdint.h>
#include <stddef.h>

typedef struct _GopInfo {
    uint64_t init;
    uint64_t fbAddress;
    uint64_t fbSize;
    uint32_t width;
    uint32_t height;
    uint32_t pixelPerScanLine;
} GopInfo;


uint32_t RGB(uint8_t R, uint8_t G, uint8_t B);
void graphics_initalize();
void graphics_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void graphics_test_graphics();
void graphics_clear(uint32_t color);
GopInfo graphics_return_gop_info();
void graphics_putchar(char c, uint32_t x, uint32_t y, uint32_t size, uint32_t color);
void graphics_print(char* s, uint32_t x, uint32_t y, uint32_t size, uint32_t color);
size_t graphics_measure_text(char* s, uint32_t fontsize);
uint8_t graphics_get_if_init();
#endif