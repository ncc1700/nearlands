#include "../../../core/term/term.h"
#include "../../includes/misc.h"



void exception_unimplemented(){
    term_write_printf(ERROR, "Unknown Exception Called, halting");
    halt_core();
}


void exception_error(){
    term_write_printf(ERROR, "Something critical has happened");
    halt_core();
}
