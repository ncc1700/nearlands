#include <stdint.h>
#include "port.h"




void LdrArEnterCharacterToSerial(const char c){
    Amd64OutByte(0x3f8, c);
}


void LdrArPrintToSerial(const char* s){
    while(*s != '\0'){
        LdrArEnterCharacterToSerial(*s);
        s++;
    }
}