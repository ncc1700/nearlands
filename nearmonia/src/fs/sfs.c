#include "sfs.h"
#include "../extern/EFI/LoadedImage.h"
#include "../extern/EFI/SimpleFileSystem.h"
#include "../extern/EFI/UefiBaseType.h"
#include "../extern/EFI/UefiMultiPhase.h"
#include "../ldr/abs.h"
#include "../ar/includes/serial.h"

static EFI_FILE_PROTOCOL* root = NULL;





boolean SFsSetupSimpleFilesystem(){
    EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID sfsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    EFI_LOADED_IMAGE_PROTOCOL* lip = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfs = NULL;

    EFI_STATUS status = LdrReturnSystemTable()->BootServices->HandleProtocol(LdrReturnImagehandle(), 
                                                                            &lipGuid, (void**)&lip);
    if(status != EFI_SUCCESS){
        ArPrintToSerial(QSTR("Couldn't access loaded image protocol\n"));
        return FALSE; // panic here
    }
    status = LdrReturnSystemTable()->BootServices->HandleProtocol(lip->DeviceHandle, 
                                                                            &sfsGuid, (void**)&sfs);
    if(status != EFI_SUCCESS){
        ArPrintToSerial(QSTR("Couldn't access simple file system protocol\n"));
        return FALSE; // panic here
    }

    sfs->OpenVolume(sfs, &root);
    return TRUE;
}

EFI_FILE_PROTOCOL* SFsOpenFile(const char* fileName, u64 mode){
    if(root == NULL) return NULL;
    wchar wFileName[256];
    QolAnsiStringToWideString(fileName, wFileName, 256);
    EFI_FILE_PROTOCOL* file = NULL;
    root->Open(root, &file, wFileName, mode, 0);
    return file;
}

char* SFsReadFileContent(EFI_FILE_PROTOCOL* file, u64* length){
    if(file == NULL) return NULL;
    char* content = NULL;
    u64 size = 16;
    u64 finalSize = 16;
    while(1){
        size = 16;
        char buffer[16];
        EFI_STATUS status = file->Read(file, &size, (void*)content);
        if(status != EFI_SUCCESS || size == 0) break;
        finalSize += size;
    }
    if(finalSize == 0) return NULL;
    EFI_STATUS status = file->SetPosition(file, 0);
    if(status != EFI_SUCCESS){
        return NULL;
    }
    status = LdrReturnSystemTable()->BootServices->AllocatePool(EfiLoaderData, finalSize, (void**)&content);
    if(status != EFI_SUCCESS){
        return NULL;
    }
    status = file->Read(file, &finalSize, content);
    if(status != EFI_SUCCESS){
        LdrReturnSystemTable()->BootServices->FreePool(content);
        return NULL;
    }
    *length = finalSize;
    return content;
}

boolean SFsCloseFile(EFI_FILE_PROTOCOL* file){
    EFI_STATUS status = file->Close(file);
    if(status != EFI_SUCCESS) return FALSE;
    return TRUE;
}