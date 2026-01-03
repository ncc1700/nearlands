#include <stdint.h>
#include "port.h"




void ArEnterCharacterToSerial(const char c){
    Amd64OutByte(0x3f8, c);
}


void ArPrintToSerial(const char* s){
    while(*s != '\0'){
        ArEnterCharacterToSerial(*s);
        s++;
    }
}