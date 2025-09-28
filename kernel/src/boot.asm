section .bss
    align 16
stackBottom:
    resb 256000
stackTop:

section .text

extern kernel_entry
global kernel_load_entry
kernel_load_entry:
    mov rsp, stackTop
    call kernel_entry
loop:
    jmp loop