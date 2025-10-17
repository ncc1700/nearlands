#include "core/kalloc.h"
#include "core/term/term.h"
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



    setup_hal();
    map_init();
    setup_generic_devices();

    term_write_printf(INFO, "Loaded Core Utilites");
    setup_smp();

    char* a = kalloc(10);
    a[0] = 'h';
    term_write_printf(INFO, "a is in memory location 0x%x", a);
    char* b = kalloc(5);
    b[0] = 'h';
    term_write_printf(INFO, "b is in memory location 0x%x", b);
    char* c = kalloc(5);
    c[0] = 'h';
    term_write_printf(INFO, "c is in memory location 0x%x", c);

    halt_core();
}
