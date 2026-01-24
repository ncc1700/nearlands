#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED










#include "../extern/EFI/SimpleFileSystem.h"
#include "../types.h"









boolean SFsSetupSimpleFilesystem();
EFI_FILE_PROTOCOL* SFsOpenFile(const char* fileName, u64 mode);
char* SFsReadFileContent(EFI_FILE_PROTOCOL* file, u64* length);
boolean SFsCloseFile(EFI_FILE_PROTOCOL* file);















#endif