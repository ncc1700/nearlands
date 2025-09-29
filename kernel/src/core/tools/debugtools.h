#ifndef DEBUGTOOLS_H_INCLUDED
#define DEBUGTOOLS_H_INCLUDED









#include <stdint.h>



void DKPRINT(char* string);
void DKPRINTLN(char* string);
void DKPRINTHEX(uint64_t hex);
void DKPRINTHEXLN(uint64_t hex);
void DKPRINTTEXTANDHEX(char* text, uint64_t hex);
void DKPRINTTEXTANDHEXLN(char* text, uint64_t hex);
void DKPRINTMULTIPLESTRINGSLN(char** text);
void DKPRINTTEXTANDHEXWITHSPACE(char* text, uint64_t hex);
void DKPRINTTEXTANDHEXWITHSPACELN(char* text, uint64_t hex);
#endif