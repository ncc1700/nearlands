#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED










#include "../types.h"



typedef enum _GraphicsContextOwners {
    OWNER_DEBUG_TERM,
    OWNER_SPLASH_SCREEN
} GraphicsContextOwners;


typedef struct _GraphicsData {
    boolean init;
    u64 framebufferBase;
    u64 framebufferSize;
    u32 width;
    u32 height;
    u32 pixelsPerScanLine;
    GraphicsContextOwners owners;
} GraphicsData;


void KeGraphicsSetup(GraphicsData* data);
GraphicsData* KeGraphicsReturnData();
void KeGraphicsClear(u64 color);
void KeGraphicsDrawRect(u32 x, u32 y, u32 width, u32 height, u64 color);
void KeGraphicsDrawChar(char c, u32 x, u32 y, u32 size, u64 color);
void KeGraphicsDrawString(const char* s, u32 x, u32 y, u32 size, u64 color);
u32 KeGraphicsMeasureTextSizeFromDefaultFont(const char* string, u32 size);










#endif