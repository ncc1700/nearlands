#include "../includes/misc.h"




// bad name but idrc
void halt_core(){
    while(1){
        asm volatile("wfi");
        continue;
    }
}