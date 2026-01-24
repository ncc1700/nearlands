#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED








#include "../types.h"

typedef enum _Status {
    TERM_STATUS_ERROR,
    TERM_STATUS_PASS,
    TERM_STATUS_WARNING,
    TERM_STATUS_INFO,
    TERM_STATUS_UNKNOWN
} Status;


void KeTermClear();
void KeTermMoveDown(u32 amount);
void KeTermPuts(boolean addNewLine, const char* string, u64 color);
void KeTermPrint(Status stat, char* string, ...);







#endif