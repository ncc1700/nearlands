section .bss
    align 16
kstackbottom:
    resb 256000
kstacktop:

section .text

extern kernel_entry
global loader_entry
loader_entry:
    
    mov rsp, kstacktop
    call kernel_entry
loop:
    jmp loop