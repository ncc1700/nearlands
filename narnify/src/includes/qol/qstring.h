#ifndef QSTRING_H_INCLUDED
#define QSTRING_H_INCLUDED






#include <types.h>
#include <mm/arena.h>


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
boolean QolCopyStringHeap(QString src, QString* dest);
boolean QolCopyStringArena(Arena* arena, QString src, QString* dest);
QString QolDupeString(QString src);
QString QolDupeStringHeap(QString src);
QString QolDupeStringArena(Arena* arena, QString src);
QString QolSliceString(QString string, u64 length);

















#endif