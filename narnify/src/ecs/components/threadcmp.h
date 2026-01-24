#ifndef THREADCMP_H_INCLUDED
#define THREADCMP_H_INCLUDED





#include "../ecs.h"

// dummy thread component


typedef struct _ThreadComponent {
    u64 (*SysThreadEntry)(void* data);
} ThreadComponent;
















#endif