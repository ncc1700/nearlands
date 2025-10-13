#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED







#include <stdint.h>
#include <stddef.h>



typedef enum _status {
    ERROR,
    WAITING,
    WARNING,
    PASS,
    INFO
} status;

void term_write_printf(status stat, char* format, ...);






#endif