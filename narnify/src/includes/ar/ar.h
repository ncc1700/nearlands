#ifndef AR_H_INCLUDED
#define AR_H_INCLUDED





#include <types.h>
#include <nrstatus.h>

typedef enum _CPUArch {
    AMD64,
    AARCH64,
    IA64
} CPUArch;



NearStatus ArInitSystem();
CPUArch ArReturnCPUArch();













#endif