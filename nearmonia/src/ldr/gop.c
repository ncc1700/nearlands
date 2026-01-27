#include <ldr/gop.h>
#include <EFI/GraphicsOutput.h>
#include <EFI/UefiBaseType.h>
#include <ldr/graphics.h>
#include <ldr/abs.h>
#include <ar/serial.h>







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