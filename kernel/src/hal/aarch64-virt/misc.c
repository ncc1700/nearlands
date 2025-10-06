#include "../includes/misc.h"
#include "../../core/term/term.h"




// bad name but idrc
void halt_core(){
    while(1){
        term_write_status(INFO, "halting system");
        asm volatile("wfi");
        continue;
    }
}