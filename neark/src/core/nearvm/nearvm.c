#include "nearvm.h"
#include <stdint.h>
#include "../memory/alloc.h"
#include "../klibc/string.h"
#include "../kernterm/kernterm.h"

#define SKIP_PER_INSTR 4

/**

    Bad prototype =(

    
*/

static inline boolean check_if_valid_register(uint64_t reg){
    if(reg >= REG_AMOUNT) return FALSE;
    return TRUE;
}

static inline boolean begin_virtual_machine(VmProgram* program){
    program->registers = (uint64_t*)core_alloc(REG_AMOUNT * sizeof(uint64_t));
    if(program->registers == NULL) return FALSE;
    program->vmMem.memory = (uint64_t*)core_alloc(program->vmMem.memsize * sizeof(uint64_t));
    if(program->vmMem.memory == NULL) return FALSE;
    memset(program->registers, 0, REG_AMOUNT * sizeof(uint64_t));
    uint64_t* arr = program->vmCode.code;
    while(*arr != OP_HALT){
        program->vmCode.codeSize++;
        arr++;
    }

    memset(program->vmMem.memory, 0, program->vmMem.memsize * sizeof(uint64_t));
    program->isCurrentlyRunning = TRUE;
    return TRUE;
}

static inline void end_virtual_machine(VmProgram* program, char* reason){
    if(program->registers != NULL){
        core_free((uint64_t*)program->registers);
    }
    if(program->registers != program->vmMem.memory){
        core_free((uint64_t*)program->vmMem.memory);
    }
    program->programCounter = 0;
    program->vmCode.codeSize = 0;
    program->isCurrentlyRunning = FALSE;

    kterm_write_printf(INFO, "This program has been killed: %s", reason);
}


static inline boolean op_add(VmProgram* program, uint64_t arg1, uint64_t arg2, uint64_t arg3){
    if(check_if_valid_register(arg1) == FALSE ||
            check_if_valid_register(arg2) == FALSE ||
            check_if_valid_register(arg3) == FALSE)
    {
        return FALSE;
    }
    program->registers[arg1] = program->registers[arg2] + program->registers[arg3];
    program->programCounter += SKIP_PER_INSTR;
    return TRUE;
}

static inline boolean op_sub(VmProgram* program, uint64_t arg1, uint64_t arg2, uint64_t arg3){
    if(check_if_valid_register(arg1) == FALSE ||
            check_if_valid_register(arg2) == FALSE ||
            check_if_valid_register(arg3) == FALSE)
    {
        return FALSE;
    }
    program->registers[arg1] = program->registers[arg2] - program->registers[arg3];
    program->programCounter += SKIP_PER_INSTR;
    return TRUE;
}

static inline boolean op_mul(VmProgram* program, uint64_t arg1, uint64_t arg2, uint64_t arg3){
    if(check_if_valid_register(arg1) == FALSE ||
            check_if_valid_register(arg2) == FALSE ||
            check_if_valid_register(arg3) == FALSE)
    {
        return FALSE;
    }
    program->registers[arg1] = program->registers[arg2] * program->registers[arg3];
    program->programCounter += SKIP_PER_INSTR;
    return TRUE;
}


static inline boolean op_div(VmProgram* program, uint64_t arg1, uint64_t arg2, uint64_t arg3){
    if(check_if_valid_register(arg1) == FALSE ||
            check_if_valid_register(arg2) == FALSE ||
            check_if_valid_register(arg3) == FALSE)
    {
        return FALSE;
    }
    if(program->registers[arg3] == 0){
        program->registers[arg1] = 0;
        return TRUE;
    }
    program->registers[arg1] = program->registers[arg2] / program->registers[arg3];
    program->programCounter += SKIP_PER_INSTR;
    return TRUE;
}


static inline boolean op_mov(VmProgram* program, uint64_t arg1, uint64_t arg2){
    if(check_if_valid_register(arg1) == FALSE || check_if_valid_register(arg2) == FALSE){
        return FALSE;
    }
    program->registers[arg1] = program->registers[arg2];
    program->programCounter += SKIP_PER_INSTR;
    return TRUE;
}


static inline boolean op_movi(VmProgram* program, uint64_t arg1, uint64_t arg2){
    if(check_if_valid_register(arg1) == FALSE){
        return FALSE;
    }

    program->registers[arg1] = arg2;
    program->programCounter += SKIP_PER_INSTR;
    return TRUE;
}



boolean vm_execute_program(VmProgram* program){
    if(program->isCurrentlyRunning == FALSE){
        begin_virtual_machine(program);
    }
    kterm_write_printf(INFO, "codeSize is %d, programCounter is %d", program->vmCode.codeSize, program->programCounter);

    while(program->programCounter < program->vmCode.codeSize){
        uint64_t op = program->vmCode.code[program->programCounter];
        if(op == OP_HALT) break;
        uint64_t arg1 = program->vmCode.code[program->programCounter + 1];
        uint64_t arg2 = program->vmCode.code[program->programCounter + 2];
        uint64_t arg3 = program->vmCode.code[program->programCounter + 3];

        switch(op){
            case OP_ADD:{
                if(op_add(program, arg1, arg2, arg3) == FALSE){
                    end_virtual_machine(program, "OP_ADD");
                    return FALSE;
                }
                break;
            }
            case OP_SUB:{
                if(op_sub(program, arg1, arg2, arg3) == FALSE){
                    end_virtual_machine(program, "OP_SUB");
                    return FALSE;
                }
                break;
            }
            case OP_MUL:{
                if(op_mul(program, arg1, arg2, arg3) == FALSE){
                    end_virtual_machine(program, "OP_MUL");
                    return FALSE;
                }
                break;
            }
            case OP_DIV:{
                if(op_div(program, arg1, arg2, arg3) == FALSE){
                    end_virtual_machine(program, "OP_DIV");
                    return FALSE;
                }
                break;
            }
            case OP_MOV:{
                if(op_mov(program, arg1, arg2) == FALSE){
                    end_virtual_machine(program, "OP_MOV");
                    return FALSE;
                }
                break;
            }
            case OP_MOVI:{
                if(op_movi(program, arg1, arg2) == FALSE){
                    end_virtual_machine(program, "OP_MOVI");
                    return FALSE;
                }
                break;
            }
            case OP_VMCALL:{
                switch (arg1) {
                    case 0:{
                        if(check_if_valid_register(arg2) == FALSE){
                            end_virtual_machine(program, "Invalid register in OP_VMCALL call 0");
                            return FALSE;
                        }
                        kterm_write_printf(INFO, "r%d has %d", arg2, program->registers[arg2]);
                        break;
                    }
                    case 1:{
                        if(check_if_valid_register(arg2) == FALSE){
                            end_virtual_machine(program, "Invalid register in OP_VMCALL call 0");
                            return FALSE;
                        }
                        char buffer[512];
                        char* string = (char*)program->vmMem.memory[program->registers[arg2]];
                        int i = 0;
                        while(*string != '\0' && i < 511){
                            buffer[i] = *string;
                            i++;
                            string++;
                        }
                        buffer[i++] = '\0';
                        kterm_write_printf(INFO, "%s", buffer);
                        break;
                    }
                    default:{
                        end_virtual_machine(program, "Unknown VMCALL");
                        return FALSE;
                    }
                }
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_STORE:{
                if(check_if_valid_register(arg1) == FALSE){
                    end_virtual_machine(program, "Invalid register in OP_STORE\n");
                    return FALSE;
                }
                if(program->vmMem.memsize <= arg2){
                    end_virtual_machine(program, "Attempted memory Overflow\n");
                    return FALSE;
                }
                program->vmMem.memory[arg2] = program->registers[arg1];
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_LOAD:{
                if(check_if_valid_register(arg2) == FALSE){
                    end_virtual_machine(program, "Invalid register in OP_LOAD\n");
                    return FALSE;
                }
                if(program->vmMem.memsize <= arg1){
                    end_virtual_machine(program, "Attempted load in address further away from memory\n");
                    return FALSE;
                }
                program->registers[arg2] = program->vmMem.memory[arg1];
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_JMP:{
                if(arg1 >= program->vmCode.codeSize){
                    end_virtual_machine(program, "Invalid JMP in OP_JMP, greater then code size");
                    return FALSE;
                }
                program->programCounter = arg1;
                break;
            }
            case OP_JEQ:{
                if(arg1 >= program->vmCode.codeSize){
                    end_virtual_machine(program, "Invalid JMP in OP_JEQ, greater then code size");
                    return FALSE;
                }
                if(check_if_valid_register(arg2) == FALSE || check_if_valid_register(arg3) == FALSE){
                    end_virtual_machine(program, "Invalid register in OP_JEQ\n");
                    return FALSE;
                }
                if(program->registers[arg2] == program->registers[arg3]){
                    program->programCounter = arg1;
                } else {
                    program->programCounter += SKIP_PER_INSTR;
                }
                break;
            }
            case OP_JNEQ:{
                if(arg1 >= program->vmCode.codeSize){
                    end_virtual_machine(program, "Invalid JMP in OP_JNEQ, greater then code size");
                    return FALSE;;
                }
                if(check_if_valid_register(arg2) == FALSE || check_if_valid_register(arg3) == FALSE){
                    end_virtual_machine(program, "Invalid register in OP_JNEQ\n");
                    return FALSE;
                }
                if(program->registers[arg2] != program->registers[arg3]){
                    program->programCounter = arg1;
                } else {
                    program->programCounter += SKIP_PER_INSTR;
                }
                break;
            }
            case OP_JGT:{
                if(arg1 >= program->vmCode.codeSize){
                    end_virtual_machine(program, "Invalid JMP in OP_JGT, greater then code size");
                    return FALSE;
                }
                if(check_if_valid_register(arg2) == FALSE || check_if_valid_register(arg3) == FALSE){
                    end_virtual_machine(program, "Invalid register in OP_JGT\n");
                    return FALSE;
                }
                if(program->registers[arg2] > program->registers[arg3]){
                    program->programCounter = arg1;
                } else {
                    program->programCounter += SKIP_PER_INSTR;
                }
                break;
            }
            case OP_JLT:{
                if(arg1 >= program->vmCode.codeSize){
                    end_virtual_machine(program, "Invalid JMP in OP_JLT, greater then code size");
                    return FALSE;
                }
                if(check_if_valid_register(arg2) == FALSE || check_if_valid_register(arg3) == FALSE){
                    end_virtual_machine(program, "Invalid register in OP_JLT\n");
                    return FALSE;
                }
                if(program->registers[arg2] < program->registers[arg3]){
                    program->programCounter = arg1;
                } else {
                    program->programCounter += SKIP_PER_INSTR;
                }
                break;
            }
            case OP_CMP:{
                if(check_if_valid_register(arg1) == FALSE ||
                    check_if_valid_register(arg2) == FALSE ||
                    check_if_valid_register(arg3) == FALSE)
                {
                    end_virtual_machine(program, "Invalid register in OP_CMP\n");
                    return FALSE;
                }
                if(program->registers[arg2] == program->registers[arg3]){
                    program->registers[arg1] = 1;
                } else {
                    program->registers[arg1] = 0;
                }
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_CMPNE:{
                if(check_if_valid_register(arg1) == FALSE ||
                    check_if_valid_register(arg2) == FALSE ||
                    check_if_valid_register(arg3) == FALSE)
                {
                    end_virtual_machine(program, "Invalid register in OP_CMPNE\n");
                    return FALSE;
                }
                if(program->registers[arg2] != program->registers[arg3]){
                    program->registers[arg1] = 1;
                } else {
                    program->registers[arg1] = 0;
                }
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_CMPGT:{
                if(check_if_valid_register(arg1) == FALSE ||
                    check_if_valid_register(arg2) == FALSE ||
                    check_if_valid_register(arg3) == FALSE)
                {
                    end_virtual_machine(program, "Invalid register in OP_CMPGT\n");
                    return FALSE;
                }
                if(program->registers[arg2] >= program->registers[arg3]){
                    program->registers[arg1] = 1;
                } else {
                    program->registers[arg1] = 0;
                }
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_CMPLT:{
                if(check_if_valid_register(arg1) == FALSE ||
                    check_if_valid_register(arg2) == FALSE ||
                    check_if_valid_register(arg3) == FALSE)
                {
                    end_virtual_machine(program, "Invalid register in OP_CMPLT\n");
                    return FALSE;
                }
                if(program->registers[arg2] <= program->registers[arg3]){
                    program->registers[arg1] = 1;
                } else {
                    program->registers[arg1] = 0;
                }
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            default:{
                kterm_write_printf(INFO, "%lu is INVALID!!!\n", op);
                end_virtual_machine(program, "Invalid Instruction\n");
                return FALSE;
            }

        }
    }
    end_virtual_machine(program, "Program exited successfully");
    return TRUE;
}
