#include "HAL/includes/misc.h"
#include "core/kernterm/kernterm.h"
#include "core/kernvid/kernvid.h"
#include "core/pmm/bitmap.h"
#include "core/pmm/freemap.h"
#include "ldrconfig.h"
#include <stddef.h>
#include <stdint.h>
#include "HAL/includes/archsetup.h"
#include "nearlands.h"
#include "HAL/includes/serial.h"

void __chkstk(){}


LoaderInfo lInfo = {0};


static inline void setup_loader_info_struct(LoaderInfo* info){
    lInfo.fb = info->fb;
    lInfo.memmap.amount = info->memmap.amount;
    lInfo.memmap.memEntries = info->memmap.memEntries;
    lInfo.memoryAmount = info->memoryAmount;
    lInfo.mode = info->mode;
    for(int i = 0; i < lInfo.memmap.amount; i++){
        lInfo.memmap.memEntries[i].base = info->memmap.memEntries[i].base;
        lInfo.memmap.memEntries[i].size = info->memmap.memEntries[i].size;
        lInfo.memmap.memEntries[i].types = info->memmap.memEntries[i].types;
    }
}

void kernel_entry(LoaderInfo* info, uint64_t stackLoc){
    setup_loader_info_struct(info);
    hal_setup_arch();
    fmp_init(&lInfo.memmap);
    kernvid_initialize(lInfo.fb);
    kernvid_clear(RGB(0, 0, 0));
    kterm_write_printf(PASS, "Nearlands Build %s %d.%d.%d running on %s",
                  VERSION_STRING, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, hal_return_arch());
    kterm_write_printf(INFO, "Memory Amount is %d, memmap amount is %d, stack loc is 0x%x", lInfo.memoryAmount, lInfo.memmap.amount, stackLoc);
    void* first = fmp_allocate_multiple_pages(10);
    void* second = fmp_allocate_multiple_pages(10);

    kterm_write_printf(INFO, "first is 0x%x", (uint64_t)first);
    kterm_write_printf(INFO, "second is 0x%x", (uint64_t)second);
    fmp_free_multiple_pages(first, 10);
    void* third = fmp_allocate_multiple_pages(10);
    kterm_write_printf(INFO, "third is 0x%x", (uint64_t)third);



    kterm_write_printf(ERROR, "Nothing else to do, halting");
    //core_panic("idk");
    hal_halt_system();
}
