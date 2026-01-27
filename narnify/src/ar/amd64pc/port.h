#ifndef PORT_H_INCLUDED
#define PORT_H_INCLUDED








#include <types.h>






void Amd64OutByte(u16 port, u8 value);
void Amd64OutWord(u16 port, u16 value);
u8 Amd64InByte(u16 port);
u16 Amd64InWord(u16 port);














#endif