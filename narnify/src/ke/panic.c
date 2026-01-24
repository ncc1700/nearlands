#include "panic.h"
#include "../ar/includes/serial.h"
#include "graphics.h"













void KePanic(const char* string){
    ArPrintToSerial("\n\n----------------------------------------------------------------\n");
    ArPrintToSerial("An error has occured!!!!\n\n");
    ArPrintToSerial(string);
    if(KeGraphicsReturnData()->init == 1){
        KeGraphicsDrawRect(0, 0, KeGraphicsReturnData()->width, 
                                KeGraphicsReturnData()->height, 0x0000FF);
        KeGraphicsDrawString("An Error Has Occured", 10, 10, 2, 0xFFFFFF);
        KeGraphicsDrawString(string, 10, 50, 1, 0xFFFFFF);
    }
    ArPrintToSerial("\n\n----------------------------------------------------------------\n");
    while(1){continue;}
}