#include "../includes/dumpreg.h"
#include "../../core/kernterm/kernterm.h"

typedef struct _Registers {
    uint64_t x0;
    uint64_t x1;
    uint64_t x2;
    uint64_t x3;
    uint64_t x4;
    uint64_t x5;
    uint64_t x6;
    uint64_t x7;
    uint64_t x8;
    uint64_t x9;
    uint64_t x10;
    uint64_t x11;
    uint64_t x12;
    uint64_t x13;
    uint64_t x14;
    uint64_t x15;
    uint64_t x16;
    uint64_t x17;
    uint64_t x18;
    uint64_t fp;
    uint64_t lr;
    uint64_t xzr;
    uint64_t esr;
    uint64_t far;
} Registers;

extern void hal_dump_and_print_registers();

void print_registers(Registers* reg){
    kterm_write_printf(INFO, "x0 - %x | x1 - %x", reg->x0, reg->x1);
    kterm_write_printf(INFO, "x2 - %x | x3 - %x", reg->x2, reg->x3);
    kterm_write_printf(INFO, "x4 - %x | x5 - %x", reg->x4, reg->x5);
    kterm_write_printf(INFO, "x6 - %x | x7 - %x", reg->x6, reg->x7);
    kterm_write_printf(INFO, "x8 - %x | x9 - %x", reg->x8, reg->x9);
    kterm_write_printf(INFO, "x10 - %x | x11 - %x", reg->x10, reg->x11);
    kterm_write_printf(INFO, "x12 - %x | x13 - %x", reg->x12, reg->x13);
    kterm_write_printf(INFO, "x14 - %x | x15 - %x", reg->x14, reg->x15);
    kterm_write_printf(INFO, "x16 - %x | x17 - %x", reg->x16, reg->x17);
    kterm_write_printf(INFO, "x18 - %x | fp - %x", reg->x18, reg->fp);
    kterm_write_printf(INFO, "xzr - %x | esr - %x", reg->xzr, reg->esr);
    kterm_write_printf(INFO, "far - %x", reg->far);
}

