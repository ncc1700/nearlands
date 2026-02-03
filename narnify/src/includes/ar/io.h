#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED








#include <types.h>





void ArIoOutByte(u16 port, u8 value);
void ArIoOutWord(u16 port, u16 value);
u8 ArIoInByte(u16 port);
u16 ArIoInWord(u16 port);









#endif