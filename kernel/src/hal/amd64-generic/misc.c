#include "../includes/misc.h"
#include "../../core/term/term.h"




// bad name but idrc
void halt_core(){
    while(1){
        asm volatile("hlt");
        continue;
    }
}