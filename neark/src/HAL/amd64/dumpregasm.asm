section .text








global hal_dump_and_print_registers
extern print_registers


hal_dump_and_print_registers:
    sub rsp, 120
    mov [rsp + 0], rax
    mov [rsp + 8], rbx
    mov [rsp + 16], rcx 
    mov [rsp + 24], rdx 
    mov [rsp + 32], rsi
    mov [rsp + 40], rbp 
    mov [rsp + 48], r8 
    mov [rsp + 56], r9
    mov [rsp + 64], r10
    mov [rsp + 72], r11 
    mov [rsp + 80], r12 
    mov [rsp + 88], r13 
    mov [rsp + 96], r14 
    mov [rsp + 104], r15 

    lea rdi, [rsp]
    call print_registers

    add rsp, 120
    ret
