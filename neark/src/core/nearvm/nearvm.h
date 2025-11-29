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
    OP_HALT,
    OP_STORE,
    OP_LOAD,
    OP_JMP,
    OP_JEQ,
    OP_JNEQ,
    OP_JGT,
    OP_JLT,
    OP_CMP,
    OP_CMPNE,
    OP_CMPGT,
    OP_CMPLT,
} VmOpCodes;

typedef struct _VmMem {
    uint64_t* memory;
    size_t memsize;
} VmMem;

typedef struct _VmCode {
    uint64_t* code;
    size_t codeSize;
} VmCode;


typedef struct _VmProgram {
    boolean isCurrentlyRunning;
    uint64_t* registers;
    VmMem vmMem;
    VmCode vmCode;
    size_t programCounter;
} VmProgram;


boolean vm_execute_program(VmProgram* program);

#endif
