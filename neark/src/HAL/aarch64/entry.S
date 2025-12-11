.global hal_loader_entry






hal_loader_entry:
    ldr x30, =sp_top
    mov sp, x30
    adrp x0, ldrInfo
    add x0, x0, :lo12:ldrInfo
    bl kernel_entry
    b .


.section .ldr
.align 16
ldrInfo:
    .space 128
