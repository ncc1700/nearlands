#ifndef DISPLAYMODEL_H_INCLUDED
#define DISPLAYMODEL_H_INCLUDED







#include <stdint.h>
#include <stddef.h>

typedef struct _ddf {
    uint32_t reserved; // key
    uint8_t (*draw_pixel)(uint32_t display, uint32_t x, uint32_t y, uint64_t color);
} ddf;


uint32_t add_display_driver(ddf* func);
uint8_t switch_to_display_driver(uint32_t key);
uint8_t draw_pixel(uint32_t display, uint32_t x, uint32_t y, uint64_t color);





#endif