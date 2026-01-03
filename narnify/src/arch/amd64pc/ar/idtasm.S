section .text
global Amd64LoadIDT










Amd64LoadIDT:
    push rbp
    mov rbp, rsp

    lidt [rel rcx]

    mov rsp, rbp
    pop rbp
    ret
