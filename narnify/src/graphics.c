#include "graphics.h"
#include "extern/BitFonts/font_8x12.h"



static GraphicsData globalGraphics = {FALSE}; // all are 0







void GraphicsSetup(GraphicsData* data){
    globalGraphics.framebufferBase = data->framebufferBase;
    globalGraphics.framebufferSize = data->framebufferSize;
    globalGraphics.height = data->height;
    globalGraphics.width = data->width;
    globalGraphics.pixelsPerScanLine = data->pixelsPerScanLine;
    globalGraphics.owners = data->owners;
    globalGraphics.init = TRUE;
}

GraphicsData* GraphicsReturnData(){
    return &globalGraphics;
}


void GraphicsClear(u64 color){
    u32* fbPtr = (u32*)globalGraphics.framebufferBase;
    for(u32 i = 0; i < globalGraphics.width; i++){
        if(i >= globalGraphics.width) continue;
        for(u32 j = 0; j < globalGraphics.height; j++){
            if(j >= globalGraphics.height) continue;
            fbPtr[j * globalGraphics.pixelsPerScanLine + i] = color;
        }
    }
}

void GraphicsDrawRect(u32 x, u32 y, u32 width, u32 height, u64 color){
    u32* fbPtr = (u32*)globalGraphics.framebufferBase;
    for(u32 i = x; i < x + width; i++){
        if(i >= globalGraphics.width) continue;
        for(u32 j = y; j < y + height; j++){
            if(j >= globalGraphics.height) continue;
            fbPtr[j * globalGraphics.pixelsPerScanLine + i] = color;
        }
    }
}


void GraphicsDrawChar(char c, u32 x, u32 y, u32 size, u64 color){
    const u8* bmp = font_8x12[c];
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < 12; j++){
            if(bmp[i] & (1 << j)){
                GraphicsDrawRect(x + j * size, y + i * size, size, size, color);
            }
        }
    }
}

void GraphicsDrawString(const char* s, u32 x, u32 y, u32 size, u64 color){
    u32 currentX = x;
    u32 currentY = y;
    u32 fsize = 8;
    while(*s != '\0'){
        GraphicsDrawChar(*s, currentX, currentY, size, color);
        currentX += size * fsize;
        s++;
    }
}

u32 GraphicsMeasureTextSizeFromDefaultFont(const char* string, u32 size){
    u32 s = 0;
    u32 fsize = 8;
    while(*string != '\0'){
        s += size * fsize;
        string++;
    }
    return s;
}