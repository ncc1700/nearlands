#include "fs.h"
#include "externheaders/efi/LoadedImage.h"
#include "externheaders/efi/ProcessorBind.h"
#include "externheaders/efi/SimpleFileSystem.h"
#include "externheaders/efi/UefiBaseType.h"
#include "graphics.h"
#include "qol.h"


static EFI_FILE_PROTOCOL *root = NULL;


void fs_initalize(EFI_HANDLE imageHandle){
    EFI_GUID lipguid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID sfsguid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_STATUS status = EFI_SUCCESS;
    EFI_LOADED_IMAGE_PROTOCOL* lip = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfs = NULL;
    status = qol_return_systab()->BootServices->HandleProtocol(imageHandle, &lipguid, (void**)&lip);
    if(status != EFI_SUCCESS){
        qol_halt_system(L"Couldn't get loaded image protocal");
    }
    status = qol_return_systab()->BootServices->HandleProtocol(lip->DeviceHandle, &sfsguid, (void**)&sfs);
    if(status != EFI_SUCCESS){
        qol_halt_system(L"Couldn't get simple file system protocol");
    }
    sfs->OpenVolume(sfs, &root);
}


EFI_FILE_PROTOCOL* fs_open_file(wchar* filename, uint64_t mode){
    EFI_FILE_PROTOCOL* file = NULL;
    root->Open(root, &file, (CHAR16*)filename, mode, 0);
    return file;
}

void fs_close_file(EFI_FILE_PROTOCOL* prot){
    root->Close(prot);
}

// THIS IS A STUB, DO NOT USE
// char* fs_read_file(EFI_FILE_PROTOCOL* file, wchar* filename){
//     char buf[16];
//     UINTN h = 64;
//     root->Read(file, &h, buf);
//     return buf; // 
// }