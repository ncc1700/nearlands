#include "HAL/includes/misc.h"
#include "core/kernterm/kernterm.h"
#include "core/kernvid/kernvid.h"
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
    kterm_write_printf(INFO, "Memory Amount is %d, memmap amount is %d", info->memoryAmount, info->memmap.amount);
    void* h = physmem_allocate_page();
    void* p = physmem_allocate_multiple_pages(3);
    void* n = physmem_allocate_page();
    kterm_write_printf(INFO, "h = 0x%x p = 0x%x", (uint64_t)h, (uint64_t)p);
    physmem_free_multiple_pages(p, 3);
    void* l = physmem_allocate_multiple_pages(5);
    void* m = physmem_allocate_multiple_pages(3);
    kterm_write_printf(INFO, "m = 0x%x", (uint64_t)m);
    kterm_write_printf(ERROR, "Nothing else to do, halting");
    hal_halt_system();
}
