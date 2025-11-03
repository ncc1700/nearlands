#include "../arch.h"









Architecture what_arch(){
    return AMD64;
}


void halt(){
    while(1){
        asm volatile("hlt");
        continue;
    }
}