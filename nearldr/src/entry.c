#include "externheaders/efi/SimpleFileSystem.h"
#include "fs.h"
#include "graphics.h"
#include "qol.h"
#include "ui.h"




int ldr_entry(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable) {
    qol_initalize_abstractions(systemTable);
    qol_puts(L"Hello World!");
    graphics_initalize();
    fs_initalize(imageHandle);
    graphics_clear(RGB(0, 0, 0));
    render_ui();
    EFI_FILE_PROTOCOL* prot = fs_open_file(L"\\nearldr.ini", EFI_FILE_MODE_READ);
    if(prot == NULL){
        qol_puts(L"doens't exist");
    }
    while(1){continue;}
    return 0;
}
