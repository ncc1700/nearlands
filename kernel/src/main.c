#include "core/term/term.h"
#include "drivers/displaymodel/displaymodel.h"
#include "drivers/setupgeneric.h"
#include "hal/includes/halinit.h"
#include "hal/includes/mem.h"
#include "hal/includes/misc.h"
#include "hal/includes/serial.h"
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
    print_char_to_serial('h');
    if(LIMINE_BASE_REVISION_SUPPORTED == 0){
        halt_core();
    }



    //DKPRINTLN("Loading kernel"); 
    setup_hal();
    setup_generic_devices();
    bitmap_init();
    setup_smp();

    // char* a = allocate_multiple_bitmaps(10);
    // a[0] = 'h';
    // term_write_printf(INFO, "a is in memory location 0x%x", a);
    // char* b = allocate_multiple_bitmaps(5);
    // b[0] = 'h';
    // term_write_printf(INFO, "b is in memory location 0x%x", b);
    // free_multiple_bitmaps(a, 10);
    // char* c = allocate_multiple_bitmaps(20);
    // c[0] = 'h';
    // term_write_printf(INFO, "c is in memory location 0x%x", c);
    //         print_char_to_serial('h');

    halt_core();
}