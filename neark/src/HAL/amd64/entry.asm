section .bss
    align 16
kstackbottom:
    resb 256000
kstacktop:

section .ldr
    align 16
ldrInfo:
    resb 12800

section .text

extern kernel_entry
global hal_loader_entry
hal_loader_entry:

    mov rsp, kstacktop
    mov rcx, ldrInfo
    call kernel_entry
loop:
    jmp loop




    ; mov rsp, kstacktop + 256000
