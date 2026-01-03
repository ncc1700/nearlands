#ifndef QOL_H_INCLUDED
#define QOL_H_INCLUDED






#include "types.h"





boolean QolInitSystem();
void *memcpy(void* dest, const void* src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);













#endif