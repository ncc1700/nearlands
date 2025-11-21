#include "panic.h"
#include "../kernterm/kernterm.h"
#include "../../HAL/includes/dumpreg.h"
#include "../../HAL/includes/misc.h"








void core_panic(char* reason){
    kterm_clear();
    kterm_write_printf(ERROR, "---------------------------------------------------------");
    kterm_write_printf(ERROR, "KERNEL PANICKED!");
    kterm_write_printf(ERROR, "STOP MESSAGE: %s", reason);
    kterm_write_printf(ERROR, "DUMPING REGISTERS");
    kterm_write_printf(ERROR, " ");
    hal_dump_and_print_registers();
    kterm_write_printf(ERROR, " ");
    kterm_write_printf(ERROR, "HALTING SYSTEM");
    kterm_write_printf(ERROR, "---------------------------------------------------------");
    while(1){hal_halt_system();}
}