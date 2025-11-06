#include "../../includes/archsetup.h"
#include "gdt.h"
#include "idt.h"





void hal_setup_arch(){
    setup_gdt();
    setup_idt();
}
