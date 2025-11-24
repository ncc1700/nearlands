#ifndef NEARVM_H_INCLUDED
#define NEARVM_H_INCLUDED





#include <stddef.h>
#include <stdint.h>
#include "../../nearlands.h"


#define REG_AMOUNT 16

typedef enum _VmOpCodes {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOV,
    OP_MOVI,
    OP_VMCALL,
    OP_HALT
} VmOpCodes;

typedef struct _VmStack {
    uint64_t* stack;
    size_t stackSize;
    uint64_t sp;
} VmStack;

typedef struct _VmCode {
    uint64_t* code;
    size_t codeSize;
} VmCode;


typedef struct _VmProgram {
    boolean isCurrentlyRunning;
    uint64_t* registers;
    VmStack vmStack;
    VmCode vmCode;
    size_t programCounter;
} VmProgram;


boolean vm_execute_program(VmProgram* program);

#endif
