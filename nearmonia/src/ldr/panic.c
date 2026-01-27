#include <ldr/panic.h>
#include <ar/serial.h>
#include <ldr/graphics.h>













void LdrPanic(QString string){
    ArPrintToSerial(QSTR("\n\n----------------------------------------------------------------\n"));
    ArPrintToSerial(QSTR("An error has occured!!!!\n\n"));
    ArPrintToSerial(string);
    if(LdrGraphicsReturnData()->init == 1){
        LdrGraphicsDrawRect(0, 0, LdrGraphicsReturnData()->width, 
                                LdrGraphicsReturnData()->height, 0x0000FF);
        LdrGraphicsDrawString(string, 10, 10, 2, 0xFFFFFF);
        LdrGraphicsDrawString(string, 10, 50, 1, 0xFFFFFF);
    }
    ArPrintToSerial(QSTR("\n\n----------------------------------------------------------------\n"));
    while(1){continue;}
}