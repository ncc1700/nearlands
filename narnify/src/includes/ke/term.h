#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED








#include <types.h>
#include <qol/qstring.h>

typedef enum _Status {
    TERM_STATUS_ERROR,
    TERM_STATUS_PASS,
    TERM_STATUS_WARNING,
    TERM_STATUS_INFO,
    TERM_STATUS_UNKNOWN,
    TERM_STATUS_IMPINFO
} Status;

typedef enum _ColorPalette {
    PALETTE_BACKGROUND,
    PALETTE_TEXT,
    PALETTE_ERROR,
    PALETTE_PASS,
    PALETTE_WARNING,
    PALETTE_INFO,
    PALETTE_IMPINFO
} ColorPalette;


void KeTermClear();
void KeTermTakeOwnershipOfKernelGraphics();
NearStatus KeTermChangeSinglePaletteColor(ColorPalette palette, u64 color);
NearStatus KeTermChangePalette(u64* palette, u64 len);
void KeTermMoveDown(u32 amount);
void KeTermPuts(boolean addNewLine, QString str, u64 color);
void KeTermPrint(Status stat, QString str, ...);







#endif