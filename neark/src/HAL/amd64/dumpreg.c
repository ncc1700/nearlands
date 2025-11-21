#include "../includes/dumpreg.h"
#include "../../core/kernterm/kernterm.h"

typedef struct _Registers {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
} Registers;

extern void hal_dump_and_print_registers();

void print_registers(Registers* reg){
    kterm_write_printf(INFO, "rax - %x | rbx - %x", reg->rax, reg->rbx);
    kterm_write_printf(INFO, "rcx - %x | rdx - %x", reg->rcx, reg->rdx);
    kterm_write_printf(INFO, "rsi - %x | rbp - %x", reg->rsi, reg->rbp);
    kterm_write_printf(INFO, "r8 - %x  | r9 - %x", reg->r8, reg->r9);
    kterm_write_printf(INFO, "r10 - %x | r11 - %x", reg->r10, reg->r11);
    kterm_write_printf(INFO, "r12 - %x | r13 - %x", reg->r12, reg->r13);
    kterm_write_printf(INFO, "r14 - %x | r15 - %x", reg->r14, reg->r15);
}




