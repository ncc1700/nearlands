section .ldr
    align 16
ldrInfo:
    resb 12800

section .text

extern kernel_entry
global hal_loader_entry
extern sp_top
hal_loader_entry:

    mov rsp, sp_top
    mov rdi, ldrInfo
    call kernel_entry
loop:
    jmp loop




    ; mov rsp, kstacktop + 256000
