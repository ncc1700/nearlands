#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED










#include "externheaders/efi/SimpleFileSystem.h"
#include "externheaders/efi/Uefi.h"
#include <stdint.h>




void fs_initalize(EFI_HANDLE imageHandle);
EFI_FILE_PROTOCOL* fs_open_file(wchar* filename, uint64_t mode);
void fs_close_file(EFI_FILE_PROTOCOL* prot);








#endif