#include "../uefi/uefi.h"
#include "arch/arch.h"
#include "graphics.h"

/*
    Thanks for POSIX-EFI for the template!

    just took it, changed the build system a bit and thats it!

*/


int main(int argc, char **argv) {
    graphics_get_gop_info();
    printf("width: %d, height: %d, fsize = %d\n", 
        graphics_return_gop_info().width, graphics_return_gop_info().height, graphics_return_gop_info().fbSize);
    printf("NearLDR Refined on %s\n", what_arch());

    graphics_test_graphics();
    sleep(1);
    graphics_clear();

    graphics_print("Nearlands Boot Manager\nIn the abyss...........", 
                   10, 10, 1, RGB(0, 255, 255));


    while(1){continue;}
    return 0;
}
