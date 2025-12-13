#include "../arch.h"
#include <stdint.h>









Architecture what_arch(){
    return AMD64;
}

void load_arch(){
    //setup_gdt();
}

void switch_stack(uint64_t stack){
    __asm__("mov %%rsp, %0" : : "r"(stack));
}



void halt(){
    while(1){
        asm volatile("hlt");
        continue;
    }
}