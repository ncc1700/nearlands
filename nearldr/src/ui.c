#include "ui.h"
#include "graphics.h"




void render_ui(Config conf){
    graphics_draw_rect(0, 0, graphics_return_gop_info().width, 30, RGB(180, 180, 180));
    graphics_draw_rect(0, 
                        graphics_return_gop_info().height - 30, 
                        graphics_return_gop_info().width, 30, RGB(180, 180, 180));

    char* headertext = "Nearlands Boot Manager";
    size_t headertextsize = graphics_measure_text(headertext, 1);
    graphics_print(headertext, (graphics_return_gop_info().width - headertextsize) / 2, 
                    14, 1, RGB(0, 0, 0));

    char* entertext = "Enter=Choose";
    graphics_print(entertext, 10, 
                    graphics_return_gop_info().height - 16, 
                    1, RGB(0, 0, 0));
    
    graphics_print("Boot Normally", 10, graphics_return_gop_info().height / 2 - 120, 
                1, RGB(255, 255, 255));
    graphics_print("Boot in Debug Mode", 10, graphics_return_gop_info().height / 2 - 80, 
                1, RGB(255, 255, 255));
    graphics_print("Reboot", 10, 
                        graphics_return_gop_info().height / 2 - 40, 1, RGB(255, 255, 255));
    graphics_print("Shutdown", 10, 
                        graphics_return_gop_info().height / 2, 1, RGB(255, 255, 255));
    graphics_print("Return to UEFI", 10, 
                        graphics_return_gop_info().height / 2 + 40, 1, RGB(255, 255, 255));
    graphics_print("<-", graphics_return_gop_info().width - 40, 
                            graphics_return_gop_info().height / 2, 1, RGB(255, 255, 255));

}


