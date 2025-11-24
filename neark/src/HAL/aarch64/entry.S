.global hal_loader_entry






hal_loader_entry:
    isb
    mrs x0, SCTLR_EL1
    bic x0, x0, (1 << 0)
    bic x0, x0, (1 << 2)
    bic x0, x0, (1 << 12)
    bic x0, x0, (1 << 11)
    msr SCTLR_EL1, x0
    isb
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
