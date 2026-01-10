section .text

global KernLoaderEntry
extern KernSystemStartup







KernLoaderEntry:
    mov rsp, kstackend
    mov rcx, ldrInfo
    jmp KernSystemStartup
.loop:
    jmp .loop



section .bss
kstack:
    resb 256000
kstackend:

section .ldr
ldrInfo:
    resb 64000
ldrInfoEnd:
