#include "displaymodel.h"
#include "../../core/tools/debugtools.h"
// currently an array and a counter, will make a linked list when i get a
// memory allocator (virtual or physical) working

static ddf ddf_drivers[13];
static uint32_t amount = 0;
static uint32_t cur_driver = 0;

uint8_t switch_to_display_driver(uint32_t key){
    if(key >= amount){
        return 1;
    }
    // TODO: clear screen before switching
    DKPRINTTEXTANDDECWITHSPACE(__FILE__, __LINE__);
    DKPRINTTEXTANDHEXLN(" Switching display to ", key);
    cur_driver = key;
    return 0;
}

uint32_t add_display_driver(ddf* func){
    ddf_drivers[amount].draw_pixel = func->draw_pixel;
    ddf_drivers[amount].reserved = amount;
    DKPRINTTEXTANDDECWITHSPACE(__FILE__, __LINE__);
    DKPRINTTEXTANDHEX(" New display driver with a key of ", amount);
    DKPRINTLN(" has been added");
    amount++;
    if(amount - 1 == 0){
        //switch_to_display_driver(amount - 1);
    }
    return amount - 1;
}

uint8_t draw_pixel(uint32_t display, uint32_t x, uint32_t y, uint64_t color){
    if(amount == 0) return 1;
    return ddf_drivers[cur_driver].draw_pixel(display, x, y, color);
}