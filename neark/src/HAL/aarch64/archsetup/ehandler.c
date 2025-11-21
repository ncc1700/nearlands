
#include "../../../core/misc/panic.h"
#include <stdint.h>
#include "../../includes/misc.h"


void exception_unimplemented(){
    *(uint64_t*)0x09000000 = 'h';
    core_panic("Unknown Exception");
    hal_halt_system();
}


void exception_error(){
    *(uint64_t*)0x09000000 = 'h';
    core_panic("Fatal exception raised by processor");
    hal_halt_system();
}
