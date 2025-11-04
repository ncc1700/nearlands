#include "config.h"
#include "externheaders/efi/SimpleFileSystem.h"
#include "fs.h"
#include "graphics.h"
#include "qol.h"
//#include "ui.h"
#include "externheaders/inih/ini.h"
#include "ui.h"



int ldr_entry(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable) {
    qol_initalize_abstractions(systemTable);
    qol_puts(L"We are ALIVE!");
    graphics_initalize();
    fs_initalize(imageHandle);
    graphics_clear(RGB(0, 0, 0));
    EFI_FILE_PROTOCOL* prot = fs_open_file(L"\\nearldr.ini", EFI_FILE_MODE_READ);
    if(prot == NULL){
        qol_halt_system(L"Couldn't find config");
    }
    char* result = fs_read_file(prot);
    Config config;
    if(ini_parse_string(result, config_parse_handler, &config) < 0){
        qol_halt_system(L"error parsing config");
    }
    render_ui(config);
    //qol_puts(config.email);
    systemTable->BootServices->FreePool((void*)result);
    while(1){continue;}
    return 0;
}
