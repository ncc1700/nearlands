#include <stdint.h>
#include "port.h"
#include "../includes/serial.h"



void ArSerialWrite(const char c){
    Amd64OutByte(0x3f8, c);
}

void ArPrintToSerialRaw(const char* s){
    while(*s != '\0'){
        ArSerialWrite(*s);
        s++;
    }
}


void ArPrintToSerial(QString str){
    for(u64 i = 0; i < str.length; i++){
        ArSerialWrite(str.buffer[i]);
    }
}