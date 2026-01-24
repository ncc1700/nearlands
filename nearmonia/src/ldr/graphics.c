#include "graphics.h"
#include "../extern/BitFonts/font_8x12.h"


static GraphicsData globalGraphics = {FALSE}; // all are 0







void LdrGraphicsSetup(GraphicsData* data){
    globalGraphics.framebufferBase = data->framebufferBase;
    globalGraphics.framebufferSize = data->framebufferSize;
    globalGraphics.height = data->height;
    globalGraphics.width = data->width;
    globalGraphics.pixelsPerScanLine = data->pixelsPerScanLine;
    globalGraphics.init = TRUE;
}

GraphicsData* LdrGraphicsReturnData(){
    return &globalGraphics;
}


void LdrGraphicsClear(u64 color){
    if(globalGraphics.init != TRUE) return;
    u32* fbPtr = (u32*)globalGraphics.framebufferBase;
    for(u32 j = 0; j < globalGraphics.height; j++){
        if(j >= globalGraphics.height) continue;
        for(u32 i = 0; i < globalGraphics.height; i++){
            if(j >= globalGraphics.height) continue;
            fbPtr[j * globalGraphics.pixelsPerScanLine + i] = color;
        }
    }
}

void LdrGraphicsDrawRect(u32 x, u32 y, u32 width, u32 height, u64 color){
    if(globalGraphics.init != TRUE) return;
    u32* fbPtr = (u32*)globalGraphics.framebufferBase;
    for(u32 j = y; j < y + height; j++){
        if(j >= globalGraphics.height) continue;
        for(u32 i = x; i < x + width; i++){
            if(i >= globalGraphics.width) continue;
            fbPtr[j * globalGraphics.pixelsPerScanLine + i] = color;
        }
    }
}


void LdrGraphicsDrawChar(char c, u32 x, u32 y, u32 size, u64 color){
    if(globalGraphics.init != TRUE) return;
    const u8* bmp = font_8x12[c];
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < 12; j++){
            if(bmp[i] & (1 << j)){
                LdrGraphicsDrawRect(x + j * size, y + i * size, size, size, color);
            }
        }
    }
}

void LdrGraphicsDrawStringRaw(const char* s, u32 x, u32 y, u32 size, u64 color){
    if(globalGraphics.init != TRUE) return;
    u32 currentX = x;
    u32 currentY = y;
    u32 fsize = 8;
    while(*s != '\0'){
        LdrGraphicsDrawChar(*s, currentX, currentY, size, color);
        currentX += size * fsize;
        s++;
    }
}

void LdrGraphicsDrawString(QString string, u32 x, u32 y, u32 size, u64 color){
    if(globalGraphics.init != TRUE) return;
    u32 currentX = x;
    u32 currentY = y;
    u32 fsize = 8;
    for(u64 i = 0; i < string.length; i++){
        LdrGraphicsDrawChar(string.buffer[i], currentX, currentY, size, color);
        currentX += size * fsize;
    }
}

u32 LdrGraphicsMeasureTextSizeFromDefaultFont(const char* string, u32 size){
    u32 s = 0;
    u32 fsize = 8;
    while(*string != '\0'){
        s += size * fsize;
        string++;
    }
    return s;
}