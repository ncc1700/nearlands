#include "nearvm.h"
#include <stdint.h>
#include "../memory/alloc.h"
#include "../klibc/string.h"
#include "../kernterm/kernterm.h"

#define SKIP_PER_INSTR 4


static inline boolean check_if_valid_register(uint64_t reg){
    if(reg >= REG_AMOUNT) return FALSE;
    return TRUE;
}

static inline boolean begin_virtual_machine(VmProgram* program){
    program->registers = (uint64_t*)core_alloc(REG_AMOUNT * sizeof(uint64_t));
    if(program->registers == NULL) return FALSE;
    program->vmStack.stack = (uint64_t*)core_alloc(program->vmStack.stackSize * sizeof(uint64_t));
    if(program->vmStack.stack == NULL) return FALSE;
    memset(program->registers, 0, REG_AMOUNT * sizeof(uint64_t));
    uint64_t* arr = program->vmCode.code;
    while(*arr != OP_HALT){
        program->vmCode.codeSize++;
        arr++;
    }

    memset(program->vmStack.stack, 0, program->vmStack.stackSize * sizeof(uint64_t));
    program->isCurrentlyRunning = TRUE;
    return TRUE;
}

static inline void end_virtual_machine(VmProgram* program){
    if(program->registers != NULL){
        core_free((uint64_t*)program->registers);
    }
    if(program->registers != program->vmStack.stack){
        core_free((uint64_t*)program->vmStack.stack);
    }
    program->programCounter = 0;
    program->vmStack.sp = 0;
    program->vmCode.codeSize = 0;
    program->isCurrentlyRunning = FALSE;
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
                if(check_if_valid_register(arg1) == FALSE ||
                    check_if_valid_register(arg2) == FALSE ||
                    check_if_valid_register(arg3) == FALSE)
                {
                    end_virtual_machine(program);
                    return FALSE;
                }
                program->registers[arg1] = program->registers[arg2] + program->registers[arg3];
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_SUB:{
                if(check_if_valid_register(arg1) == FALSE ||
                    check_if_valid_register(arg2) == FALSE ||
                    check_if_valid_register(arg3) == FALSE)
                {
                    end_virtual_machine(program);
                    return FALSE;
                }
                program->registers[arg1] = program->registers[arg2] - program->registers[arg3];
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_MUL:{
                if(check_if_valid_register(arg1) == FALSE ||
                    check_if_valid_register(arg2) == FALSE ||
                    check_if_valid_register(arg3) == FALSE)
                {
                    end_virtual_machine(program);
                    return FALSE;
                }
                program->registers[arg1] = program->registers[arg2] * program->registers[arg3];
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_DIV:{
                if(check_if_valid_register(arg1) == FALSE ||
                    check_if_valid_register(arg2) == FALSE ||
                    check_if_valid_register(arg3) == FALSE)
                {
                    end_virtual_machine(program);
                    return FALSE;
                }
                if(program->registers[arg3] == 0){
                    program->registers[arg1] = 0;
                } else {
                    program->registers[arg1] = program->registers[arg2] / program->registers[arg3];
                }
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_MOV:{
                if(check_if_valid_register(arg1) == FALSE || check_if_valid_register(arg2) == FALSE){
                    end_virtual_machine(program);
                    return FALSE;
                }
                program->registers[arg1] = program->registers[arg2];
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_MOVI:{
                if(check_if_valid_register(arg1) == FALSE){
                    end_virtual_machine(program);
                    return FALSE;
                }

                program->registers[arg1] = arg2;
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            case OP_VMCALL:{
                switch (arg1) {
                    case 0:{
                        if(check_if_valid_register(arg2) == FALSE){
                            end_virtual_machine(program);
                            return FALSE;
                        }
                        kterm_write_printf(INFO, "r%d has %d", arg2, program->registers[arg2]);
                    }
                    default:{
                        end_virtual_machine(program);
                        return FALSE;
                    }
                }
                program->programCounter += SKIP_PER_INSTR;
                break;
            }
            default:{
                end_virtual_machine(program);
                return FALSE;
            }

        }
    }
    end_virtual_machine(program);
    return TRUE;
}
