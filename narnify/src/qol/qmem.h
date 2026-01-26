#ifndef QMEM_H_INCLUDED
#define QMEM_H_INCLUDED









#include "../types.h"


#define KB(x) ((u64)(x) << 10)
#define MB(x) ((u64)(x) << 20)
#define GB(x) ((u64)(x) << 30)
#define TB(x) ((u64)(x) << 40)

#define TO_KB(x) ((u64)(x) >> 10)
#define TO_MB(x) ((u64)(x) >> 20)
#define TO_GB(x) ((u64)(x) >> 30)
#define TO_TB(x) ((u64)(x) >> 40)


void *memcpy(void* dest, const void* src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);


















#endif