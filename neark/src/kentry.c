#include "HAL/includes/idmap.h"
#include "HAL/includes/misc.h"
#include "core/kernterm/kernterm.h"
#include "core/kernvid/kernvid.h"
#include "core/memory/alloc.h"
#include "core/memory/physmem.h"
#include "core/nearvm/nearvm.h"
#include "ldrconfig.h"
#include <stddef.h>
#include <stdint.h>
#include "HAL/includes/archsetup.h"
#include "nearlands.h"



void __chkstk(){}

void kernel_entry(LoaderInfo* info){
    hal_setup_arch();
    initialize_physmem_allocator(&info->memmap, info->memoryAmount);
    setup_identity_map();
    kernvid_initialize(info->fb);
    kernvid_clear(RGB(0, 0, 0));
    kterm_write_printf(PASS, "Nearlands Build %s %d.%d.%d running on %s",
                VERSION_STRING, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, hal_return_arch());
    kterm_write_printf(INFO, "Memory Amount is %d, memmap amount is %d", info->memoryAmount, info->memmap.amount);
    char c = 'h';
    uint64_t code[] = {
        OP_MOVI, 0, 10, 0,
        OP_STORE, 0, 10, 0,
        OP_LOAD, 10, 1, 0,
        OP_VMCALL, 0, 1, 0,
        OP_HALT, 0, 0, 0
    };
    VmCode vmCode = {code, 0};
    VmProgram program;
    program.programCounter = 0;
    program.vmMem.memsize = 512;
    program.vmCode = vmCode;
    program.isCurrentlyRunning = FALSE;

    vm_execute_program(&program);

    kterm_write_printf(ERROR, "Nothing else to do, halting");
    hal_halt_system();
}
