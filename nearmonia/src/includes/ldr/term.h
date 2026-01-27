#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED








#include <types.h>
#include <qol/qstring.h>

typedef enum _Status {
    TERM_STATUS_ERROR,
    TERM_STATUS_PASS,
    TERM_STATUS_WARNING,
    TERM_STATUS_INFO,
    TERM_STATUS_UNKNOWN,
    TERM_STATUS_IMPINFO
} Status; 


void LdrTermClear();
void LdrTermMoveDown(u32 amount);
void LdrTermPuts(boolean addNewLine, QString str, u64 color);
void LdrTermPrint(Status stat, QString str, ...);







#endif