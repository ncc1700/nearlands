.global hal_loader_entry






hal_loader_entry:
    ldr x30, =kstackend
    mov sp, x30
    ldr x0, =ldrInfo
    bl kernel_entry
    b .

.section .bss
.align 16
kstack:
    .space 256000
kstackend:


.section .ldr
.align 16
ldrInfo:
    .space 128
