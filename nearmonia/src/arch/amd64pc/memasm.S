
section .text

global LdrMmUpdateCr3














LdrMmUpdateCr3:
    push rbp
    mov rbp, rsp

    mov cr3, rcx

    mov rsp, rbp
    pop rbp
    ret