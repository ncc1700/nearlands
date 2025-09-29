#include "../includes/port.h"
#include <stdint.h>




void out_byte(uint16_t port, uint8_t value){
    asm volatile("out %1, %0" : : "%d"(port), "a"(value));
}

void out_word(uint16_t port, uint16_t value){
    asm volatile("out %1, %0" : : "%d"(port), "a"(value));
}

void out_dword(uint16_t port, uint32_t value){
    asm volatile("out %1, %0" : : "%d"(port), "a"(value));
}

uint8_t in_byte(uint16_t port){
    uint8_t result;
    asm volatile("in %1, %0" : "=a"(result) : "d"(port));
    return result;
}

uint16_t in_word(uint16_t port){
    uint16_t result;
    asm volatile("in %1, %0" : "=a"(result) : "d"(port));
    return result;
}

uint32_t in_dword(uint16_t port){
    uint32_t result;
    asm volatile("in %1, %0" : "=a"(result) : "d"(port));
    return result;
}