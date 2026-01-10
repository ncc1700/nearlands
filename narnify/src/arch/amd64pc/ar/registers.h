#ifndef REGISTERS_H_INCLUDED
#define REGISTERS_H_INCLUDED









#include "../../../types.h"



typedef struct _Register {
    u64 rax;
    u64 rbx;
    u64 rcx;
    u64 rdx;
    u64 rsi;
    u64 rdi;
    u64 r8;
    u64 r9;
    u64 r10;
    u64 r11;
    u64 r12;
    u64 r13;
    u64 r14;
    u64 r15;
} Register;















#endif