section .text
global load_idt





section .text





load_idt:
    push rbp
    mov rbp, rsp

    lidt [rel rcx]

    mov rsp, rbp
    pop rbp
    ret
