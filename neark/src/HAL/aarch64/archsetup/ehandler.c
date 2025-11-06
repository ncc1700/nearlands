#include "../../../core/kernterm/kernterm.h"
#include <stdint.h>
#include "../../includes/misc.h"


void exception_unimplemented(){
    *(uint64_t*)0x09000000 = 'h';
    kterm_write_printf(ERROR, "Unknown Exception Called, halting");
    hal_halt_system();
}


void exception_error(){
    *(uint64_t*)0x09000000 = 'h';
    kterm_write_printf(ERROR, "Something critical has happened");
    hal_halt_system();
}
