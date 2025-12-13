#include "HAL/includes/misc.h"
#include "core/kernterm/kernterm.h"
#include "core/kernvid/kernvid.h"
#include "core/klibc/string.h"

#include "core/misc/panic.h"
#include "ldrconfig.h"
#include <stddef.h>
#include <stdint.h>
#include "HAL/includes/archsetup.h"
#include "nearlands.h"
#include "HAL/includes/serial.h"

void __chkstk(){}


void kernel_entry(LoaderInfo* info){
    hal_print_char_to_serial('h');
    //hal_setup_arch();
    kernvid_initialize(info->fb);
    kernvid_clear(RGB(0, 0, 0));
    kterm_write_printf(PASS, "Nearlands Build %s %d.%d.%d running on %s",
                  VERSION_STRING, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, hal_return_arch());
    kterm_write_printf(INFO, "Memory Amount is %d, memmap amount is %d", info->memoryAmount, info->memmap.amount);    
    kterm_write_printf(ERROR, "Nothing else to do, halting");
    hal_halt_system();
}
