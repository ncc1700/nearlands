#include "../includes/halinit.h"
#include "gdt/gdt.h"
#include "idt/idt.h"




void setup_hal(){
    setup_gdt();
    setup_idt();
}