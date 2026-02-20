section .text


global ArRdmsr
global ArWrmsr


; u64 ArRdmsr(u64 id);
ArRdmsr:
    push rbp
    mov rbp, rsp
    rdmsr
    shl rdx, 32
    or rax, rdx
    mov rsp, rbp
    pop rbp
    ret

; u64 ArWrmsr(u64 id, u64 high, u64 low);
ArWrmsr:
    push rbp
    mov rbp, rsp
    mov eax, r8d
    wrmsr
    mov rsp, rbp
    pop rbp
    ret