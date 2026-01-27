#include <qol/qstring.h>
#include <mm/heap.h>
#include <ar/serial.h>
#include <string.h>





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
boolean QolCopyStringHeap(QString src, QString* dest){
    if(src.buffer == NULL) return FALSE;
    dest->buffer = MmAllocateFromHeap((src.length + 1) * sizeof(u8));
    if(dest->buffer == NULL) return FALSE;
    memcpy(dest->buffer, src.buffer, src.length * sizeof(u8));
    dest->buffer[src.length] = '\0'; 
    dest->length = src.length;
    return TRUE;
}

// dest.buffer is allocated from an arena, deal with it
boolean QolCopyStringArena(Arena* arena, QString src, QString* dest){
    if(src.buffer == NULL) return FALSE;
    dest->buffer = MmPushMemoryFromArena(arena, (src.length + 1) * sizeof(u8));
    if(dest->buffer == NULL) return FALSE;
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
QString QolDupeStringHeap(QString src){
    QString result = {0};
    if(src.buffer == NULL) goto EXIT;
    result.buffer = MmAllocateFromHeap((src.length + 1) * sizeof(u8));
    if(result.buffer == NULL) goto EXIT;
    memcpy(result.buffer, src.buffer, src.length * sizeof(u8));
    result.buffer[src.length] = '\0'; 
    result.length = src.length;
    EXIT:
    return result;
}

// result.buffer is allocated from an arena, deal with it
QString QolDupeStringArena(Arena* arena, QString src){
    QString result = {0};
    if(src.buffer == NULL) goto EXIT;
    result.buffer = MmPushMemoryFromArena(arena, (src.length + 1) * sizeof(u8));
    if(result.buffer == NULL) goto EXIT;;
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


// boolean QolQStrToCStr(QString* qstr, char* cstr, u64 cstrLength){
    
//     return TRUE;
// }