#ifndef QSTRING_H_INCLUDED
#define QSTRING_H_INCLUDED






#include "../types.h"


typedef struct _QString {
    u8* buffer;
    u64 length;
} QString;

u64 QolGetStringSize(u8* string);
u64 QolGetStringSizeEx(u8* string, boolean addNewLine);
QString QolInitStringEx(u8* str, u64 length);
QString QolInitString(u8* str);


#define Q(x) (u8*)x
#define C(x) (char*)x
#define QSTR(x) QolInitString(Q(x))














#endif