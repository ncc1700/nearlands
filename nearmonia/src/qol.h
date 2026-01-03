#ifndef QOL_H_INCLUDED
#define QOL_H_INCLUDED








#include "types.h"



#include "extern/EFI/Uefi.h"



void QolSetupQolAbstractions(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systab);
EFI_SYSTEM_TABLE* QolReturnSystemTable();
EFI_HANDLE QolReturnImagehandle();
void QolWideStringToAnsiString(const wchar* wideString, char* string, u32 strSize);
void QolAnsiStringToWideString(const char* ansiString, wchar* string, u32 strSize);
void QolSerialFormatPrint(const char* string, ...);
void *memcpy(void* dest, const void* src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void QolPanic(const char* string);








#endif