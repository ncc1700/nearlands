#include "arch/arch.h"
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
    qol_printf("We are ALIVE!");
    graphics_initalize();
    fs_initalize(imageHandle);
    load_arch();
    setup_paging();
    graphics_clear(RGB(0, 0, 0));
    EFI_FILE_PROTOCOL* prot = fs_open_file(L"\\nearldr.ini", EFI_FILE_MODE_READ);
    if(prot == NULL){
        qol_halt_system("Couldn't find config");
    }
    char* result = fs_read_file(prot);
    Config config;
    if(ini_parse_string(result, config_parse_handler, &config) < 0){
        qol_halt_system("error parsing config");
    }
    systemTable->BootServices->FreePool((void*)result);
    render_ui(config, imageHandle);
    
    return 0;
}
