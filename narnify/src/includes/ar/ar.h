#ifndef AR_H_INCLUDED
#define AR_H_INCLUDED





#include <types.h>

typedef enum _CPUArch {
    AMD64,
    AARCH64,
    IA64
} CPUArch;



boolean ArInitSystem();
CPUArch ArReturnCPUArch();













#endif