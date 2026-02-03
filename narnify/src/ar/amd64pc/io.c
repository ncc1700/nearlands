#include "port.h"
#include <ar/io.h>






void ArIoOutByte(u16 port, u8 value){
    Amd64OutByte(port, value);
}

void ArIoOutWord(u16 port, u16 value){
    Amd64OutWord(port, value);
}

u8 ArIoInByte(u16 port){
    return Amd64InByte(port);
}

u16 ArIoInWord(u16 port){
    return Amd64InWord(port);
}

