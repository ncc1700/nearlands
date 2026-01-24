#include "gop.h"
#include "../extern/EFI/GraphicsOutput.h"
#include "../extern/EFI/UefiBaseType.h"
#include "graphics.h"
#include "abs.h"
#include "../ar/includes/serial.h"







boolean LdrSetupGOP(){
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = NULL;
    EFI_STATUS status = LdrReturnSystemTable()->BootServices->LocateProtocol(&gopGuid, 
                                                            NULL, (void**)&gop);
    if(status != EFI_SUCCESS){
        ArPrintToSerial(QSTR("LocateProtocol for GOP failed"));
        return FALSE;
    }
    // we just use the normal resolution the system is already at, nothing special
    // TODO: add params so one can choose the resolutions/modes



    // we pass all of our junky GOP data to the graphics system

    GraphicsData gData = {
        TRUE,
        gop->Mode->FrameBufferBase,
        gop->Mode->FrameBufferSize,
        gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution,
        gop->Mode->Info->PixelsPerScanLine
    };
    LdrGraphicsSetup(&gData);
    return TRUE;
}