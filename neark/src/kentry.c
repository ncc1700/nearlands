#include "HAL/includes/idmap.h"
#include "HAL/includes/misc.h"
#include "core/kernterm/kernterm.h"
#include "core/kernvid/kernvid.h"
#include "core/memory/alloc.h"
#include "core/klibc/string.h"

#include "core/memory/physmem.h"
#include "core/nearvm/nearvm.h"
#include "externheaders/lil/lil.h"
#include "ldrconfig.h"
#include <stddef.h>
#include <stdint.h>
#include "HAL/includes/archsetup.h"
#include "nearlands.h"


void __chkstk(){}

lil_value_t sys_puts(lil_t lil, size_t size, lil_value_t * value){
    kterm_write_printf(INFO, "This was called!");
    //kterm_write_printf(INFO, "%s", value);
    return 0;   
}
void kernel_entry(LoaderInfo* info){
    hal_setup_arch();
    initialize_physmem_allocator(&info->memmap, info->memoryAmount);
    setup_identity_map();
    kernvid_initialize(info->fb);
    kernvid_clear(RGB(0, 0, 0));
    kterm_write_printf(PASS, "Nearlands Build %s %d.%d.%d running on %s",
                VERSION_STRING, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, hal_return_arch());
    kterm_write_printf(INFO, "Memory Amount is %d, memmap amount is %d", info->memoryAmount, info->memmap.amount);
    
    lil_t lil = lil_new();
    lil_register(lil, "sys_puts\n", sys_puts);
    char buffer[] = "print \"poo\"\n";
    lil_value_t res = lil_parse(lil, buffer, strlen(buffer), 0);
    const char* strres = lil_to_string(res);
    //kterm_write_printf(ERROR, "%s\n", strres);

    //if (strres[0])
    kterm_write_printf(ERROR, "Nothing else to do, halting");
    hal_halt_system();
}
