#include <stdint.h>
#include "port.h"




void hal_print_char_to_serial(char c){
    out_byte(0x3f8, c);
}


void hal_print_string_to_serial(char* s){
    while(*s != '\0'){
        hal_print_char_to_serial(*s);
        s++;
    }
}