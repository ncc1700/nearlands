#include "ke/term.h"
#include "nrstatus.h"
#include <ke/nrstatusutil.h>
#include "qol/qstring.h"
#include "types.h"
#include <ke/panic.h>
#include <ar/serial.h>
#include <ke/graphics.h>






// void KePanicInternal(QString string){
    
//     while(1){continue;}
// }


static u64 panicScreenPalette[] = {
    [PALETTE_BACKGROUND] = 0x000080,
    [PALETTE_TEXT] = 0xFFFFFF,
    [PALETTE_ERROR] = 0x800000,
    [PALETTE_PASS] = 0x008000,
    [PALETTE_WARNING] = 0x999900,
    [PALETTE_INFO] = 0xFFFFFF,
    [PALETTE_IMPINFO] = 0xb03c03
};



static inline void PanicInitialize(){
    KeGraphicsReturnData()->owners = OWNER_DEBUG_TERM;
    u64 paletteLen = ARR_LENGTH(panicScreenPalette);
    // we don't care if it fails, we are too deep in rn
    KeTermChangePalette(panicScreenPalette, paletteLen);
    KeTermClear();
    KeTermPrint(TERM_STATUS_UNKNOWN, QSTR("--------------------PANIC--------------------"));
    KeTermPrint(TERM_STATUS_ERROR, QSTR("The kernel has panicked"));
    KeTermMoveDown(2);
    KeTermPrint(TERM_STATUS_INFO, QSTR("INFO ABOUT THE PANIC!"));
    KeTermMoveDown(2);
}

static inline void PanicEnd(){
    KeTermMoveDown(2);
    KeTermPrint(TERM_STATUS_INFO, QSTR("Please report this to the developers"));
    KeTermPrint(TERM_STATUS_UNKNOWN, QSTR("--------------------PANIC--------------------"));
    while(1){continue;}
}


void KePanic(NearStatus panicStatus){
    PanicInitialize();
    QString str = KeNrStatusToString(panicStatus);
    if(str.length <= 3){
        KeTermPrint(TERM_STATUS_INFO, QSTR("Error code is %d"), panicStatus);
    } else {
        KeTermPuts(FALSE, QSTR("      "), panicScreenPalette[PALETTE_BACKGROUND]);
        KeTermPuts(TRUE, str, panicScreenPalette[PALETTE_TEXT]);
    }
    PanicEnd();
}


void KePanicStr(QString str){
    PanicInitialize();
    KeTermPuts(FALSE, QSTR("      "), panicScreenPalette[PALETTE_BACKGROUND]);    
    KeTermPuts(TRUE, str, panicScreenPalette[PALETTE_TEXT]);
    PanicEnd();
}