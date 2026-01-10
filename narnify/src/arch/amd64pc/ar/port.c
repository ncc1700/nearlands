#include "port.h"



void Amd64OutByte(u16 port, u8 value){
    asm volatile("out %1, %0" : : "%d"(port), "a"(value));
}

void Amd64OutWord(u16 port, u16 value){
    asm volatile("out %1, %0" : : "%d"(port), "a"(value));
}

u8 Amd64InByte(u16 port){
    u8 result;
    asm volatile("in %1, %0" : "=a"(result) : "d"(port));
    return result;
}

u16 Amd64InWord(u16 port){
    u16 result;
    asm volatile("in %1, %0" : "=a"(result) : "d"(port));
    return result;
}

