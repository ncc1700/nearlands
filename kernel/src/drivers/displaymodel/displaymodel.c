#include "displaymodel.h"

#define DISPLAY_AMOUNT 13


// allows up to 13 display driver to be loaded
static ddf ddf_drivers[DISPLAY_AMOUNT];
static uint32_t amount = 0;
static uint32_t cur_driver = 0;

uint8_t switch_to_display_driver(uint32_t key){
    if(key >= amount){
        return 1;
    }
    cur_driver = key;
    return 0;
}

uint32_t add_display_driver(ddf* func){
    if(amount >=DISPLAY_AMOUNT){
        // will return 14, which is out of bounds meaning
        // of course, it failed. Unless some magic happened =D
        return 14;
    }
    ddf_drivers[amount].draw_pixel = func->draw_pixel;
    ddf_drivers[amount].get_width = func->get_width;
    ddf_drivers[amount].get_height = func->get_height;

    ddf_drivers[amount].reserved = amount;
    amount++;
    if(amount - 1 == 0){
        switch_to_display_driver(amount - 1);
    }
    return amount - 1;
}

uint8_t draw_pixel(uint32_t display, uint32_t x, uint32_t y, uint64_t color){
    if(amount == 0) return 1;
    return ddf_drivers[cur_driver].draw_pixel(display, x, y, color);
}

uint64_t get_width(uint32_t display){
    if(amount == 0) return 1;
    return ddf_drivers[cur_driver].get_width(display);
}

uint64_t get_height(uint32_t display){
    if(amount == 0) return 1;
    return ddf_drivers[cur_driver].get_height(display);
}
