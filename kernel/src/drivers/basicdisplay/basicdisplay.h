#ifndef BASICDISPLAY_H_INCLUDED
#define BASICDISPLAY_H_INCLUDED




#include <stdint.h>




uint8_t basic_display_draw_pixel(uint32_t display, uint32_t x, uint32_t y, uint64_t color);
uint8_t setup_basic_display();



#endif