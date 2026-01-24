#include "qstring.h"
#include <string.h>
#include "../ldr/abs.h"



u64 QolGetStringLength(u8* string){
    if(string == NULL) return 0;
    u8* copy = string;
    u64 length = 0;
    while(*copy != '\0'){
        length++;
        copy++;
    }
    return length;
}

u64 QolGetStringLengthEx(u8* string, boolean addNewLine){
    if(string == NULL) return 0;
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
    return QolInitStringEx(str, QolGetStringLength(str));
}

// just copies the pointer reference
void QolCopyString(QString src, QString* dest){
    dest->buffer = src.buffer;
    dest->length = src.length;
}

// dest.buffer is allocated from the kernel heap, free it
boolean QolCopyStringPool(QString src, QString* dest){
    if(src.buffer == NULL) return FALSE;
    EFI_STATUS status = LdrReturnSystemTable()->BootServices->AllocatePool(EfiLoaderData, 
                    (src.length + 1) * sizeof(u8), (void**)&dest->buffer);
    if(status != EFI_SUCCESS) return FALSE;
    memcpy(dest->buffer, src.buffer, src.length * sizeof(u8));
    dest->buffer[src.length] = '\0'; 
    dest->length = src.length;
    return TRUE;
}



// just copies the pointer reference
QString QolDupeString(QString src){
    QString result = {0};
    result.buffer = src.buffer;
    result.length = src.length;
    return result;
}

// result.buffer is allocated from the kernel heap, free it
QString QolDupeStringPool(QString src){
    QString result = {0};
    if(src.buffer == NULL) goto EXIT;
    EFI_STATUS status = LdrReturnSystemTable()->BootServices->AllocatePool(EfiLoaderData, 
                    (src.length + 1) * sizeof(u8), (void**)&result.buffer);
    if(status != EFI_SUCCESS) goto EXIT;
    memcpy(result.buffer, src.buffer, src.length * sizeof(u8));
    result.buffer[src.length] = '\0'; 
    result.length = src.length;
    EXIT:
    return result;
}

 

QString QolSliceString(QString string, u64 length){
    QString result = {0};
    if(string.buffer == NULL) goto EXIT;
    if(length >= string.length) return string;
    result.buffer = string.buffer; 
    result.length = length;
    EXIT:
    return result;
}

void QolWideStringToAnsiString(const wchar* wideString, char* string, u32 strSize){
    const wchar* sCopy = wideString;
    u32 count = 0;
    while(*sCopy != L'\0' && count < strSize - 1){
        string[count] = (char)*sCopy;
        count++;
        sCopy++;
    }
    string[count] = '\0';
}

void QolAnsiStringToWideString(const char* ansiString, wchar* string, u32 strSize){
    const char* sCopy = ansiString;
    u32 count = 0;
    while(*sCopy != '\0' && count < strSize - 1){
        string[count] = (wchar)*sCopy;
        count++;
        sCopy++;
    }
    string[count] = L'\0';
}

// boolean QolQStrToCStr(QString* qstr, char* cstr, u64 cstrLength){
    
//     return TRUE;
// }