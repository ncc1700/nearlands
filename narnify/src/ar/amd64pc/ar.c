#include <ar/ar.h>
#include "gdt.h"
#include "idt.h"





boolean ArInitSystem(){
    Amd64SetupGDT();
    Amd64SetupIDT();
    return TRUE; // we will do more here soon dw
}

CPUArch ArReturnCPUArch(){
    return AMD64;
}