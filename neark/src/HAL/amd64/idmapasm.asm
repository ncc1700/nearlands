


section .text
global load_cr3



load_cr3:
    push rbp
    mov rbp, rsp

    mov cr3, rdi

    mov rsp, rbp
    pop rbp
    ret