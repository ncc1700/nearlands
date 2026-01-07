#include "fs.h"
#include "extern/EFI/LoadedImage.h"
#include "extern/EFI/SimpleFileSystem.h"
#include "extern/EFI/UefiBaseType.h"
#include "extern/EFI/UefiMultiPhase.h"
#include "qol.h"


static EFI_FILE_PROTOCOL* root = NULL;





boolean LdrFsSetupFilesystem(){
    EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID sfsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    EFI_LOADED_IMAGE_PROTOCOL* lip = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfs = NULL;

    EFI_STATUS status = QolReturnSystemTable()->BootServices->HandleProtocol(QolReturnImagehandle(), 
                                                                            &lipGuid, (void**)&lip);
    if(status != EFI_SUCCESS){
        QolSerialFormatPrint("Couldn't access loaded image protocol\n");
        return FALSE; // panic here
    }
    status = QolReturnSystemTable()->BootServices->HandleProtocol(lip->DeviceHandle, 
                                                                            &sfsGuid, (void**)&sfs);
    if(status != EFI_SUCCESS){
        QolSerialFormatPrint("Couldn't access simple file system protocol\n");
        return FALSE; // panic here
    }

    sfs->OpenVolume(sfs, &root);
    return TRUE;
}

EFI_FILE_PROTOCOL* LdrFsOpenFile(const char* fileName, u64 mode){
    if(root == NULL) return NULL;
    wchar wFileName[256];
    QolAnsiStringToWideString(fileName, wFileName, 256);
    EFI_FILE_PROTOCOL* file = NULL;
    root->Open(root, &file, wFileName, mode, 0);
    return file;
}

char* LdrFsReadFileContent(EFI_FILE_PROTOCOL* file, u64* length){
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
    status = QolReturnSystemTable()->BootServices->AllocatePool(EfiLoaderData, finalSize, (void**)&content);
    if(status != EFI_SUCCESS){
        return NULL;
    }
    status = file->Read(file, &finalSize, content);
    if(status != EFI_SUCCESS){
        QolReturnSystemTable()->BootServices->FreePool(content);
        return NULL;
    }
    *length = finalSize;
    return content;
}

boolean LdrFsCloseFile(EFI_FILE_PROTOCOL* file){
    EFI_STATUS status = file->Close(file);
    if(status != EFI_SUCCESS) return FALSE;
    return TRUE;
}