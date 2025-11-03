#include "../arch.h"








Architecture what_arch(){
    return AARCH64;
}

void halt(){
    while(1){
        asm volatile("wfi");
        continue;
    }
}