#include "../includes/misc.h"







void hal_halt_system(){
    while(1){
        asm volatile("wfi");
        continue;
    }
}
