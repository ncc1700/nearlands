#include <ar/ar.h>
#include "gdt.h"
#include "idt.h"
#include "nrstatus.h"





NearStatus ArInitSystem(){
    Amd64SetupGDT();
    Amd64SetupIDT();
    return STATUS_SUCCESS; // we will do more here soon dw
}

CPUArch ArReturnCPUArch(){
    return AMD64;
}