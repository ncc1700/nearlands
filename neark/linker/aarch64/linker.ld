ENTRY(hal_loader_entry)

SECTIONS {
    . = 0x0;
    .text : {
        *(.text)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
    . = ALIGN(8);
    . += 0x30000;
    sp_top = .;
    stack_size = 0x30000;
}