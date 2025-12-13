.global hal_loader_entry






hal_loader_entry:

    adr x30, kstackend
    mov sp, x30
    adr x0, ldrInfo
    bl kernel_entry
    b .


.section .bss
kstack:
    .skip 256000
    .align 16
kstackend:

.section .ldr
.align 16
ldrInfo:
    .space 128
