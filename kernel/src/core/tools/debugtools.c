#include "debugtools.h"
#include "../../hal/includes/serial.h"
#include "../klibc/string.h"



void DKPRINT(char* string){
    while(*string != '\0'){
        print_char_to_serial(*string);
        string++;
    }
}

void DKPRINTLN(char* string){
    DKPRINT(string);
    print_char_to_serial('\n');
}


void DKPRINTHEX(uint64_t hex){
    char buffer[90];
    DKPRINT("0x");
    xtoa(hex, buffer, 90);
    DKPRINT(buffer);
}

void DKPRINTHEXLN(uint64_t hex){
    char buffer[90];
    DKPRINT("0x");
    xtoa(hex, buffer, 90);
    DKPRINTLN(buffer);
}

void DKPRINTTEXTANDHEX(char* text, uint64_t hex){
    DKPRINT(text);
    DKPRINTHEX(hex);
}

void DKPRINTTEXTANDHEXLN(char* text, uint64_t hex){
    DKPRINT(text);
    DKPRINTHEXLN(hex);
}

void DKPRINTTEXTANDHEXWITHSPACE(char* text, uint64_t hex){
    DKPRINT(text);
    DKPRINT(":");
    DKPRINTHEX(hex);
}

void DKPRINTTEXTANDHEXWITHSPACELN(char* text, uint64_t hex){
    DKPRINT(text);
    DKPRINT(":");
    DKPRINTHEXLN(hex);
}

void DKPRINTMULTIPLESTRINGS(char** text){
    while(*text != NULL){
        DKPRINT(*text);
        DKPRINT(" ");
        text++;
    }
}

void DKPRINTMULTIPLESTRINGSLN(char** text){
    DKPRINTMULTIPLESTRINGS(text);
    print_char_to_serial('\n');
}  

void DKPRINTDEC(uint64_t decimal){
    char buffer[90];
    itoa(decimal, buffer, 90);
    DKPRINT(buffer);
}

void DKPRINTDECLN(uint64_t decimal){
    char buffer[90];
    itoa(decimal, buffer, 90);
    DKPRINTLN(buffer);
}