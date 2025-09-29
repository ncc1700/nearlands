global load_gdt

load_gdt:
    push rbp             
    mov rbp, rsp 

    lgdt [rdi]           

    mov ax, 0x10         
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push 0x08             
    lea rax, [rel .flush] 
    push rax
    retfq                

.flush:
    mov rsp, rbp          
    pop rbp   
    ret