#ifndef KERNVID_H_INCLUDED
#define KERNVID_H_INCLUDED



#include <stdint.h>
#include "../../ldrconfig.h"
#include <stddef.h>


typedef struct _KernVid {
    uint64_t init;
    uint64_t fbAddress;
    uint64_t fbSize;
    uint32_t width;
    uint32_t height;
    uint32_t pixelPerScanLine;
} KernVid;


uint32_t RGB(uint8_t R, uint8_t G, uint8_t B);
uint8_t kernvid_get_if_init();
void kernvid_initalize(Framebuffer fb);
void kernvid_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void kernvid_test_graphics();
void kernvid_clear(uint32_t color);
KernVid kernvid_return_info();
void kernvid_putchar(char c, uint32_t x, uint32_t y, uint32_t size, uint32_t color);
void kernvid_print(char* s, uint32_t x, uint32_t y, uint32_t size, uint32_t color);
size_t kernvid_measure_text(char* s, uint32_t fontsize);

#endif
