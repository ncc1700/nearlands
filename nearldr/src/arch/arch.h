#ifndef ARCH_H_INCLUDED
#define ARCH_H_INCLUDED












#include "../externheaders/efi/Uefi.h"
#include <stdint.h>


typedef enum _Architecture {
    AMD64,
    AARCH64
} Architecture;



Architecture what_arch();
void load_arch();
void setup_paging();
void switch_stack(uint64_t stack);
void halt();












#endif