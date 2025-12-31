#include "gop.h"
#include "extern/EFI/GraphicsOutput.h"
#include "extern/EFI/UefiBaseType.h"
#include "graphics.h"
#include "qol.h"
#include "types.h"









boolean LdrSetupGOP(){
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = NULL;
    EFI_STATUS status = QolReturnSystemTable()->BootServices->LocateProtocol(&gopGuid, 
                                                            NULL, (void**)&gop);
    if(status != EFI_SUCCESS){
        QolUefiFormatPrint("LocateProtocol for GOP failed with EFI_STATUS of 0x%x", status);
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
    GraphicsSetup(&gData);
    return TRUE;
}