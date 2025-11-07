#include "../includes/misc.h"







void hal_halt_system(){
    while(1){
        asm volatile("wfi");
        continue;
    }
}


char* hal_return_arch(){
    return "AARCH64";
}
