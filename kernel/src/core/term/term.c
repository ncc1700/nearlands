#include "term.h"
#include "external/font8x16.h"
#include <stdint.h>
#include "../../drivers/displaymodel/displaymodel.h"
#include "../klibc/string.h"

typedef struct _term {
    uint64_t ypos;
    uint64_t xpos;
} term;

static term terminal = {0};

void draw_rect(uint32_t display, uint32_t x, uint32_t y, uint32_t height, uint32_t width, uint64_t color){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            draw_pixel(display, x + j, y + i, color);
        }
    }
}

void term_putchar_anywhere_display(uint32_t display, char c, uint32_t x, uint32_t y, uint32_t size, uint64_t color){
    if (c < 32 || c > 126) {
        return;
    }
    const uint8_t *bitmap = kfont[c - 32]; 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            if (bitmap[i] & (1 << j)) { 
                draw_rect(display, x + j * size, y + i * size, size, size, color);
            }
        }
    }
}


void term_putchar_anywhere(char c, uint32_t x, uint32_t y, uint32_t size, uint64_t color){
    term_putchar_anywhere_display(0, c, x, y, size, color);
}

void term_write_anywhere_display(uint32_t display, int x, int y, char* s, int size, uint64_t color){
    int c;
    int current_x = x; 
    int current_y = y;
    int line_height = size * 10; 

    while ((c = *s++)) { 
        if (c == '\n') {
            current_y += line_height; 
            current_x = x; 
            terminal.ypos += 10;
        } else {
            term_putchar_anywhere_display(display, c, current_x, current_y, size, color);
            current_x += size * 10; 
        }
    }
}

void term_write_anywhere(int x, int y, char* s, int size, uint64_t color){
    term_write_anywhere_display(0, x, y, s, size, color);
}

void term_clear(){
    for(int x = 0; x < get_width(0); x++){
        for(int y = 0; y < get_height(0); y++){
            draw_pixel(0, x, y, 0x000000);
        }
    }
    terminal.ypos = 10;
    terminal.xpos = 10;
}

void term_write(char* s, uint64_t color){
    if(terminal.ypos == 0 && terminal.xpos == 0){
        term_clear();
    }
    term_write_anywhere(terminal.xpos, terminal.ypos, s, 1, color);
    terminal.xpos += (strlen(s) * 10);
    terminal.ypos += 10;
    terminal.xpos = 10;
    if(terminal.ypos >= get_height(0) - 10){
        terminal.ypos = 0;
        terminal.xpos = 0;
    }
}


void term_write_no_line(char* s, uint64_t color){
    if(terminal.ypos == 0 && terminal.xpos == 0){
        term_clear();
        terminal.ypos = 10;
        terminal.xpos = 10;
    }
    term_write_anywhere(terminal.xpos, terminal.ypos, s, 1, color);
    terminal.xpos += (strlen(s) * 10);
    if(terminal.xpos >= get_width(0) - 10){
        terminal.ypos += 10;
    }
    // terminal.ypos += 10;
    // if(terminal.ypos >= get_height(0) - 10){
    //     terminal.ypos = 0;
    // }
}

void term_write_status(status stat, char* s){
    switch(stat){
        case ERROR:
            term_write_no_line("[-] ", 0xff0000);
            term_write(s, 0xffffff);
            break;
        case PASS:
            term_write_no_line("[+] ", 0x00ff00);
            term_write(s, 0xffffff);
            break;
        case INFO:
            term_write_no_line("[!] ", 0xff00ff);
            term_write(s, 0xffffff);
            break;
        case WARNING:
            term_write_no_line("[?] ", 0xffff00);
            term_write(s, 0xffffff);
            break;
        case WAITING:
            term_write_no_line("[.] ", 0xffff);
            term_write(s, 0xffffff);
            break;
    }
}

void term_write_status_nonewline(status stat, char* s){
    switch(stat){
        case ERROR:
            term_write_no_line("[-] ", 0xff0000);
            term_write(s, 0xffffff);
            break;
        case PASS:
            term_write_no_line("[+] ", 0x00ff00);
            term_write(s, 0xffffff);
            break;
        case INFO:
            term_write_no_line("[!] ", 0xff00ff);
            term_write(s, 0xffffff);
            break;
        case WARNING:
            term_write_no_line("[?] ", 0xffff00);
            term_write(s, 0xffffff);
            break;
        case WAITING:
            term_write_no_line("[.] ", 0xffff);
            term_write(s, 0xffffff);
            break;
    }
}