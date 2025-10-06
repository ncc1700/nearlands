#include "core/term/term.h"
#include "drivers/displaymodel/displaymodel.h"
#include "drivers/setupgeneric.h"
#include "hal/includes/halinit.h"
#include "hal/includes/mem.h"
#include "hal/includes/misc.h"
#include "hal/includes/smp.h"
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
    draw_pixel(0, 10, 10, 0xfffffff);
    //DKPRINTLN("Loading kernel"); 
    setup_hal();
    setup_generic_devices();
    chunk_allocator_setup();
    setup_smp();
    
    if(LIMINE_BASE_REVISION_SUPPORTED == 0){
        term_write_status(ERROR,"Unknown Base Revision");
        halt_core();
    }
    for(int i = 0; i < 10000; i++){
        term_write_status(ERROR, "h");
    }



    halt_core();
}