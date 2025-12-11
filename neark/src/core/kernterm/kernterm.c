#include <stdint.h>
#include "kernterm.h"
#include "../kernvid/kernvid.h"
#include "../klibc/string.h"
#include "../../externheaders/mini-printf/mini-printf.h"
#include "../../HAL/includes/serial.h"
typedef struct _term {
    uint64_t ypos;
    uint64_t xpos;
} term;

static term terminal = {0};



void kterm_clear(){
    if(kernvid_get_if_init() == 0) return;
    kernvid_clear(RGB(0, 0, 0));
    terminal.ypos = 10;
    terminal.xpos = 10;
}



static inline void kterm_write(char* s, uint32_t color){
    if(kernvid_get_if_init() == 0) goto SKIP_GRAPH;
    if(terminal.ypos == 0 && terminal.xpos == 0){
        kernvid_clear(RGB(0, 0, 0));
    }
    kernvid_print(s, terminal.xpos, terminal.ypos, 1, color);
    terminal.ypos += 10;
    terminal.xpos = 10;
    if(terminal.ypos >= kernvid_return_info().height - 10){
        terminal.ypos = 0;
        terminal.xpos = 0;
    }
    SKIP_GRAPH:
    hal_print_string_to_serial(s);
    hal_print_char_to_serial('\n');
}


void kterm_write_no_line(char* s, uint32_t color){
    if(kernvid_get_if_init() == 0) goto SKIP_GRAPH;
    if(terminal.ypos == 0 && terminal.xpos == 0){
        kterm_clear();
        terminal.ypos = 10;
        terminal.xpos = 10;
    }
    kernvid_print(s, terminal.xpos, terminal.ypos, 1, color);
    terminal.xpos += (strlen(s) * 10);
    if(terminal.xpos >= kernvid_return_info().width - 10){
        terminal.ypos += 10;
    }
    SKIP_GRAPH:
    hal_print_string_to_serial(s);
    // terminal.ypos += 10;
    // if(terminal.ypos >= get_height(0) - 10){
    //     terminal.ypos = 0;
    // }
}

void kterm_write_printf(status stat, char* format, ...){
    char s[1024];
    va_list args;
    va_start(args, format);
    mini_vsnprintf(s, sizeof(s), format, args);
    va_end(args);
    switch(stat){
        case ERROR:
            kterm_write_no_line("[-] ", RGB(255, 0, 0));
            kterm_write(s, RGB(255, 255, 255));
            break;
        case PASS:
            kterm_write_no_line("[+] ", RGB(0, 255, 0));
            kterm_write(s, RGB(255, 255, 255));
            break;
        case INFO:
            kterm_write_no_line("[!] ", RGB(255, 0, 255));
            kterm_write(s, RGB(255, 255, 255));
            break;
        case WARNING:
            kterm_write_no_line("[?] ", RGB(255, 255, 0));
            kterm_write(s, RGB(255, 255, 255));
            break;
        case WAITING:
            kterm_write_no_line("[.] ", RGB(100, 255, 255));
            kterm_write(s, RGB(255, 255, 255));
            break;
    }
}
