#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED










#include <stdint.h>
#include <stddef.h>



char* strcpy_s(char* src, char* dest, size_t size);
char* strcpy(char* src, char* dest);
int strcmp(const char* str1, const char* str2);
int strsplit(char* str, char delimeter, char* res1, 
            size_t res1_size, char* res2, size_t res2_size);
size_t strlen(const char* string);
uint8_t xtoa(uint64_t integer, char* string, uint16_t size);
uint8_t itoa(uint64_t integer, char* string, uint16_t size);
uint64_t atoi_u64(const char* string);
int atoi(const char* string);
uint64_t atox_u64(const char* string);
void* memcpy(void* dest, const void* src, size_t size);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
char* strchr(const char *str, int search_str);
int isspace(char c);
//char* strdup(const char* string);
//uint8_t atow(const char* string, wchar* result, uint64_t resultsize);
#endif