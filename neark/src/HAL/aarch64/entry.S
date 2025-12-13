.global hal_loader_entry






hal_loader_entry:

    adr x30, sp_top
    adr x1, sp_top
    b kernel_entry
    b .

