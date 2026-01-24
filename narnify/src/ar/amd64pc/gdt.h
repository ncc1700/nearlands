#ifndef GDT_H_INCLUDED
#define GDT_H_INCLUDED







#include <stdint.h>





void Amd64SetupGDT();
void Amd64LoadGDT(void* gdt);






#endif