
section .text

global MmUpdateCr3














MmUpdateCr3:
    push rbp
    mov rbp, rsp

    mov cr3, rcx

    mov rsp, rbp
    pop rbp
    ret