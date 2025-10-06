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


void term_putchar_anywhere(char c, uint32_t x, uint32_t y, uint32_t size, uint64_t color);
void term_write_anywhere(int x, int y, char* s, int size, uint64_t color);
void term_clear();
void term_write(char* s, uint64_t color);
void term_write_no_line(char* s, uint64_t color);
void term_write_status(status stat, char* s);
void term_write_status_nonewline(status stat, char* s);







#endif