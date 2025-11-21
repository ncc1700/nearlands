#include "HAL/includes/idmap.h"
#include "HAL/includes/misc.h"
#include "core/kernterm/kernterm.h"
#include "core/kernvid/kernvid.h"
#include "core/memory/alloc.h"
#include "core/memory/physmem.h"
#include "ldrconfig.h"
#include <stddef.h>
#include <stdint.h>
#include "HAL/includes/archsetup.h"
#include "nearlands.h"



void __chkstk(){}

void kernel_entry(LoaderInfo* info){
    hal_setup_arch();
    initialize_physmem_allocator(&info->memmap, info->memoryAmount);
    kernvid_initialize(info->fb);
    kernvid_clear(RGB(0, 0, 0));
    kterm_write_printf(PASS, "Nearlands Build %s %d.%d.%d running on %s",
                VERSION_STRING, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, hal_return_arch());
    setup_identity_map();
    kterm_write_printf(INFO, "Memory Amount is %d, memmap amount is %d", info->memoryAmount, info->memmap.amount);
    void* h = core_alloc(10);
    void* p = core_alloc(20);
    void* n =core_alloc(10);
    kterm_write_printf(INFO, "h = 0x%x p = 0x%x", (uint64_t)h, (uint64_t)p);
    core_free(p);
    void* l = core_alloc(30);
    void* m = core_alloc(20);
    kterm_write_printf(INFO, "m = 0x%x", (uint64_t)m);
    kterm_write_printf(ERROR, "Nothing else to do, halting");
    hal_halt_system();
}
