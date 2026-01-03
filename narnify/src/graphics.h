#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED










#include "types.h"



typedef struct _GraphicsData {
    boolean init;
    u64 framebufferBase;
    u64 framebufferSize;
    u32 width;
    u32 height;
    u32 pixelsPerScanLine;
} GraphicsData;


void GraphicsSetup(GraphicsData* data);
GraphicsData* GraphicsReturnData();
void GraphicsDrawRect(u32 x, u32 y, u32 width, u32 height, u64 color);
void GraphicsDrawChar(char c, u32 x, u32 y, u32 size, u64 color);
void GraphicsDrawString(const char* s, u32 x, u32 y, u32 size, u64 color);
u32 GraphicsMeasureTextSizeFromDefaultFont(const char* string, u32 size);










#endif