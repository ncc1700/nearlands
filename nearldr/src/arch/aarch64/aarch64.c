#include "../arch.h"
#include <stdint.h>








Architecture what_arch(){
    return AARCH64;
}


void load_arch(){
    return;
}

void switch_stack(uint64_t stack){
    __asm__("mov sp, %0" : : "r"(stack));
}

void halt(){
    while(1){
        asm volatile("wfi");
        continue;
    }
}