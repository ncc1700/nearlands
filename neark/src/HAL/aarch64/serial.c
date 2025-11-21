#include <stdint.h>


#define UART_ADDR 0x09000000

void hal_print_char_to_serial(char c){
    *(volatile char*)UART_ADDR = c;
}


void hal_print_string_to_serial(char* s){
    while(*s != '\0'){
        hal_print_char_to_serial(*s);
        s++;
    }
}