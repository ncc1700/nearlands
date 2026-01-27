#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED










#include <types.h>
#include <qol/qstring.h>


typedef struct _GraphicsData {
    boolean init;
    u64 framebufferBase;
    u64 framebufferSize;
    u32 width;
    u32 height;
    u32 pixelsPerScanLine;
} GraphicsData;


void LdrGraphicsSetup(GraphicsData* data);
GraphicsData* LdrGraphicsReturnData();
void LdrGraphicsClear(u64 color);
void LdrGraphicsDrawRect(u32 x, u32 y, u32 width, u32 height, u64 color);
void LdrGraphicsDrawChar(char c, u32 x, u32 y, u32 size, u64 color);
void LdrGraphicsDrawStringRaw(const char* s, u32 x, u32 y, u32 size, u64 color);
void LdrGraphicsDrawString(QString string, u32 x, u32 y, u32 size, u64 color);
u32 LdrGraphicsMeasureTextSizeFromDefaultFont(const char* string, u32 size);










#endif