#include "gdt.h"
#include "idt.h"











void ArSetupArchitecture(){
    Amd64SetupGDT();
    Amd64SetupIDT();
}