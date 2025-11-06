#include "core/kernterm/kernterm.h"
#include "core/kernvid/kernvid.h"
#include "ldrconfig.h"
#include <stddef.h>
#include <stdint.h>
#include "HAL/includes/archsetup.h"



void __chkstk(){}

void kernel_entry(LoaderInfo* info){
    hal_setup_arch();
    kernvid_initalize(info->fb);
    kernvid_clear(RGB(0, 0, 0));
    kterm_write_printf(PASS, "We are in the kernel!");
    kterm_write_printf(INFO, "Mode is %d, memmap amount is %d", info->mode, info->memmap.amount);

    while(1){continue;}
}
