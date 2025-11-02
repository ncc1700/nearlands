.global loader_entry






loader_entry:
    ldr x30, =kstackend
    mov sp, x30
    bl kernel_entry
    b .

.section .bss
.align 16
kstack:
    .space 40960
kstackend: