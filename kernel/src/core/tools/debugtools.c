#include "debugtools.h"
#include "../../hal/includes/serial.h"




void DKPRINT(char* string){
    while(*string != '\0'){
        print_char_to_serial(*string);
        string++;
    }
}

void DKPRINTLN(char* string){
    DKPRINT(string);
    print_char_to_serial('\n');
}



