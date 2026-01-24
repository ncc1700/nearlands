#include "qstring.h"






u64 QolGetStringSize(u8* string){
    u8* copy = string;
    u64 length = 0;
    while(*copy != '\0'){
        length++;
        copy++;
    }
    return length;
}

u64 QolGetStringSizeEx(u8* string, boolean addNewLine){
    u8* copy = string;
    u64 length = 0;
    while(*copy != '\0'){
        length++;
        copy++;
    }
    if(addNewLine == TRUE) return length + 1;
    else return length;
}

QString QolInitStringEx(u8* str, u64 length){
    QString qstr = {str, length};
    return qstr;
}

QString QolInitString(u8* str){
    return QolInitStringEx(str, QolGetStringSize(str));
}

