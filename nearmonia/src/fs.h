#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED










#include "extern/EFI/SimpleFileSystem.h"
#include "types.h"









boolean LdrFsSetupFilesystem();
EFI_FILE_PROTOCOL* LdrFsOpenFile(const char* fileName, u64 mode);
char* LdrFsReadFileContent(EFI_FILE_PROTOCOL* file, u64* length);
boolean LdrFsCloseFile(EFI_FILE_PROTOCOL* file);















#endif