
.global setup_identity_map






// not really an "identity map" i just turn off the MMU =D
setup_identity_map:
    sub sp, sp, #16
    isb
    mrs x0, SCTLR_EL1
    bic x0, x0, (1 << 0)
    bic x0, x0, (1 << 2)
    bic x0, x0, (1 << 12)
    bic x0, x0, (1 << 11)
    msr SCTLR_EL1, x0
    isb
    add sp, sp, #16
    ret