section .text
global load_idt










load_idt:
    push rbp
    mov rbp, rsp

    lidt [rel rdi]

    mov rsp, rbp
    pop rbp
    ret