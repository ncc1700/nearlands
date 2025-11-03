#include "ui.h"
#include "graphics.h"




void render_ui(){
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
}