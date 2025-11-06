#include "HAL/includes/misc.h"
#include "core/kernterm/kernterm.h"
#include "core/kernvid/kernvid.h"
#include "ldrconfig.h"
#include <stddef.h>
#include <stdint.h>
#include "HAL/includes/archsetup.h"



void __chkstk(){}

void kernel_entry(LoaderInfo* info){
    hal_setup_arch();
    kernvid_initialize(info->fb);
    kernvid_clear(RGB(0, 0, 0));
    kterm_write_printf(PASS, "We are in the kernel!");
    kterm_write_printf(INFO, "Memory Amount is %d, memmap amount is %d", info->memoryAmount, info->memmap.amount);
    for(int i = 0; i < info->memmap.amount; i++){
        if(info->memmap.memEntries[i].types == EfiConventionalMemory){
            kterm_write_printf(INFO, "0x%x with size of %d can be used", info->memmap.memEntries[i].base, info->memmap.memEntries[i].size);
        }
    }
    kterm_write_printf(ERROR, "Nothing else to do, halting");
    hal_halt_system();
}
