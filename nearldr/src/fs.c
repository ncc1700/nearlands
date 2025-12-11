#include "fs.h"
#include "externheaders/efi/LoadedImage.h"
#include "externheaders/efi/ProcessorBind.h"
#include "externheaders/efi/SimpleFileSystem.h"
#include "externheaders/efi/UefiBaseType.h"
#include "externheaders/efi/UefiMultiPhase.h"
#include "graphics.h"
#include "qol.h"
#include <stdint.h>


static EFI_FILE_PROTOCOL *root = NULL;


void fs_initalize(EFI_HANDLE imageHandle){
    EFI_GUID lipguid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID sfsguid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_STATUS status = EFI_SUCCESS;
    EFI_LOADED_IMAGE_PROTOCOL* lip = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfs = NULL;
    status = qol_return_systab()->BootServices->HandleProtocol(imageHandle, &lipguid, (void**)&lip);
    if(status != EFI_SUCCESS){
        qol_halt_system("Couldn't get loaded image protocal");
    }
    status = qol_return_systab()->BootServices->HandleProtocol(lip->DeviceHandle, &sfsguid, (void**)&sfs);
    if(status != EFI_SUCCESS){
        qol_halt_system("Couldn't get simple file system protocol");
    }
    sfs->OpenVolume(sfs, &root);
}


EFI_FILE_PROTOCOL* fs_open_file(wchar* filename, uint64_t mode){
    EFI_FILE_PROTOCOL* file = NULL;
    root->Open(root, &file, (CHAR16*)filename, mode, 0);
    return file;
}

void fs_close_file(EFI_FILE_PROTOCOL* file){
    root->Close(file);
}

// very janky ik =D
char* fs_read_file(EFI_FILE_PROTOCOL* file){
    EFI_STATUS status;
    char* result;
    UINTN size = 16;
    UINTN finalsize = 0;
    while (1) {
        size = 16;
        char buf[16];
        status = file->Read(file, &size, buf);
        if(status != EFI_SUCCESS || size == 0){
            break;
        }
        finalsize += size;
    }
    if(finalsize == 0) return NULL;
    status = file->SetPosition(file, 0);
    if(status != EFI_SUCCESS) return NULL;
    status = qol_return_systab()->BootServices->AllocatePool(EfiLoaderData, finalsize, (void**)&result);
    if(status != EFI_SUCCESS) return NULL;
    status = file->Read(file, &finalsize, result);
    if(status != EFI_SUCCESS) return NULL;
    return result; 
}