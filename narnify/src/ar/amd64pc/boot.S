section .text

global ArArchitectureEntry
extern KeSystemStartup







ArArchitectureEntry:
    mov rsp, kstackend
    mov rcx, ldrInfo
    jmp KeSystemStartup
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
