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
    . = ALIGN(16);
    sp_top = .;
    . += 0x30000;
    stack_size = 0x30000;
}