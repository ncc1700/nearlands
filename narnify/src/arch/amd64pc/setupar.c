#include "ar/gdt.h"
#include "ar/idt.h"











void ArSetupArchitecture(){
    Amd64SetupGDT();
    Amd64SetupIDT();
}