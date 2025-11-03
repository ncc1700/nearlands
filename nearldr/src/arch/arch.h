#ifndef ARCH_H_INCLUDED
#define ARCH_H_INCLUDED












#include "../externheaders/efi/Uefi.h"


typedef enum _Architecture {
    AMD64,
    AARCH64
} Architecture;



Architecture what_arch();
void halt();












#endif