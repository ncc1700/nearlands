#ifndef KERNTERM_H_INCLUDED
#define KERNTERM_H_INCLUDED






typedef enum _status {
    ERROR,
    WAITING,
    WARNING,
    PASS,
    INFO
} status;

void kterm_clear();
void kterm_write_printf(status stat, char* format, ...);



#endif
