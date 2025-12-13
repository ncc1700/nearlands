
#include "../../../core/misc/panic.h"
#include <stdint.h>
#include "../../includes/misc.h"
#include "../../includes/serial.h"

void exception_unimplemented(){
    hal_print_string_to_serial("\nunknown exception\n");
    core_panic("Unknown Exception");
    hal_halt_system();
}


void exception_error(){
    hal_print_string_to_serial("\nfatal exception\n");
    core_panic("Fatal exception raised by processor");
    hal_halt_system();
}
