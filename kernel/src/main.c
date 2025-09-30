#include "core/tools/debugtools.h"
#include "drivers/displaymodel/displaymodel.h"
#include "drivers/setupgeneric.h"
#include "hal/includes/halinit.h"
#include "hal/includes/misc.h"
#include "limine.h"
#include <stddef.h>
#include <stdint.h>


__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;


void kernel_entry(void) {
    DKPRINTLN("Loading kernel"); 
    setup_hal();
    setup_generic_devices();

    if(LIMINE_BASE_REVISION_SUPPORTED == 0){
        DKPRINTLN("UNKNOWN BASE REVISION");
        halt_core();
    }
    
    
    for(int x = 90; x < 200; x++){
        for(int y = 100; y < 300; y++){
            draw_pixel(0, x, y, 0xff00ff);
        }
    }

    for(int x = 500; x < 700; x++){
        for(int y = 400; y < 500; y++){
            draw_pixel(0, x, y, 0x00ffff);
        }
    }
    
    halt_core();
}