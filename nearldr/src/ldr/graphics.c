#include "graphics.h"
#include "../uefi/uefi.h"
#include "../externheaders/font8x8_basic.h"




GopInfo info = {0};


uint32_t RGB(uint8_t R, uint8_t G, uint8_t B){
    return (R << 16) | (G << 8) | (B);
}

void graphics_get_gop_info(){
    efi_guid_t guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    efi_status_t status = EFI_SUCCESS;
    efi_gop_t *gop = NULL;
    status = BS->LocateProtocol(&guid, NULL, (void**)&gop);

    if(status != EFI_SUCCESS){
        printf("Failure Retreiving GOP info, returned with EFI_STATUS of %d\n", status);
        while(1){continue;}
    }

    info = (GopInfo){
        gop->Mode->FrameBufferBase,
        gop->Mode->FrameBufferSize,
        gop->Mode->Information->HorizontalResolution,
        gop->Mode->Information->VerticalResolution,
        gop->Mode->Information->PixelsPerScanLine
    };
}

void graphics_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color){
    volatile uint32_t* fb = (volatile uint32_t*)info.fbAddress;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fb[(y + i) * info.pixelPerScanLine + (x + j)] = color;
        }
    }
} 


void graphics_test_graphics(){
    volatile uint32_t* fb = (volatile uint32_t*)info.fbAddress;
    for(int i = 0; i < info.width; i++){
        for(int j = 0; j < info.height; j++){
            fb[j * info.pixelPerScanLine + i] = RGB(j, i, j);
        }
    }
} 

void graphics_clear(){
    volatile uint32_t* fb = (volatile uint32_t*)info.fbAddress;
    for(int i = 0; i < info.width; i++){
        for(int j = 0; j < info.height; j++){
            fb[j * info.pixelPerScanLine + i] = RGB(0, 0, 0);
        }
    }
} 


// use this instead, PLEASE do not do "extern GopInfo info"
GopInfo graphics_return_gop_info(){
    return info;
}

void graphics_putchar(char c, uint32_t x, uint32_t y, uint32_t size, uint32_t color){
    if (c < 32 || c > 126) {
        return;
    }
    const uint8_t *bitmap = font8x8_basic[c - 32]; 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (bitmap[i] & (1 << j)) { 
                graphics_draw_rect(x + j * size, y + i * size, size, size, color);
            }
        }
    }
}


void graphics_print(char* s, uint32_t x, uint32_t y, uint32_t size, uint32_t color){
    uint8_t c;
    uint32_t current_x = x; 
    uint32_t current_y = y;

    while ((c = *s++)) { 
        if (c == '\n') {
            current_y += size * 10; 
            current_x = x; 
        } else {
            graphics_putchar(c, current_x, current_y, size, color);
            current_x += size * 8; 
        }
    }
}