#include "../displaymodel/displaymodel.h"
#include "../../limine.h"
#include "../../core/term/term.h"
#include <stdint.h>

// our generic display driver is using the framebuffer limine provides us

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    LIMINE_FRAMEBUFFER_REQUEST,
    0
};

static struct limine_framebuffer** fbp = NULL;

static uint64_t amount_of_display = 0;

static inline uint8_t basic_display_draw_pixel(uint32_t display, uint32_t x, uint32_t y, uint64_t color){
    if(display >= amount_of_display){
        return 2;
    }
    if(fbp == NULL){
        return 3;
    }
    volatile uint32_t *fb = fbp[display]->address;
    fb[y * (fbp[display]->pitch / 4) + x] = color;
    return 0;
}  

static inline uint64_t basic_display_get_width(uint32_t display){  
    return fbp[display]->width;
}  

static inline uint64_t basic_display_get_height(uint32_t display){  
    return fbp[display]->height;
}  

uint8_t setup_basic_display(){
    if(framebuffer_request.response == NULL){
        // if this ends up being flagged (or the other under) 
        // my goal is to begin a UART interface instead

        //term_write_status(ERROR, "Framebuffer Request failed");
        return 1;
    }
    if(framebuffer_request.response->framebuffer_count < 1){
        //term_write_status(ERROR, "Framebuffer count less then one");
        return 2;
    }
    amount_of_display = framebuffer_request.response->framebuffer_count;
    fbp = framebuffer_request.response->framebuffers;
    ddf d = {
        0,
        basic_display_draw_pixel,
        basic_display_get_width,
        basic_display_get_height
    };
    uint32_t key = add_display_driver(&d);
    switch_to_display_driver(key);
    return 0;
}

 