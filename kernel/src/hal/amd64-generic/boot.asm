section .bss
    align 16
kstackbottom:
    resb 256000
kstacktop:

section .text

extern kernel_entry
global kernel_load_entry
kernel_load_entry:
    
    mov rsp, kstacktop
    call kernel_entry
loop:
    jmp loop