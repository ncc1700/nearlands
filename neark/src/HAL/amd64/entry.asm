section .text

extern kernel_entry
global hal_loader_entry
extern sp_top


hal_loader_entry:
    mov rsp, sp_top
    sub rsp, 8
    mov rdi, rcx
    mov rsi, sp_top
    call kernel_entry
loop:
    jmp loop