#ifndef QSTRING_H_INCLUDED
#define QSTRING_H_INCLUDED






#include "../types.h"
#include "../extern/EFI/Uefi.h"


typedef struct _QString {
    u8* buffer;
    u64 length;
} QString;

#define Q(x) (u8*)x
#define C(x) (char*)x
#define QSTR(x) QolInitString(Q(x))

u64 QolGetStringLength(u8* string);
u64 QolGetStringLengthEx(u8* string, boolean addNewLine);
QString QolInitStringEx(u8* str, u64 length);
QString QolInitString(u8* str);
void QolCopyString(QString src, QString* dest);
boolean QolCopyStringPool(QString src, QString* dest);
QString QolDupeString(QString src);
QString QolDupeStringPool(QString src);
QString QolSliceString(QString string, u64 length);
void QolWideStringToAnsiString(const wchar* wideString, char* string, u32 strSize);
void QolAnsiStringToWideString(const char* ansiString, wchar* string, u32 strSize);















#endif