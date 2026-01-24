#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED





#include "../../qol/qstring.h"




void ArSerialWrite(const char c);
void ArPrintToSerialRaw(const char* s);
void ArPrintToSerial(QString str);













#endif