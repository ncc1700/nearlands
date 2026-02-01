
section .text

global MmUpdateCr3
global MmInvalidatePage













MmUpdateCr3:
    push rbp
    mov rbp, rsp

    mov cr3, rcx

    mov rsp, rbp
    pop rbp
    ret

MmInvalidatePage:
    push rbp
    mov rbp, rsp

    invlpg [rcx]

    mov rsp, rbp
    pop rbp
    ret