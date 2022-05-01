global __cli
global __sti
global __pit_eoi
global __tools_int_test

__cli:
    cli
    ret
__sti:
    sti
    ret
__pit_eoi:
    mov al, 0x20
    out 0x20, al
    ret
__tools_int_test:
    push rbp
    mov rbp, rsp
    mov eax, edi
    mov byte [rbp-4], al
    movzx eax, byte [rbp-4]
    int 0
    nop
    pop rbp
    ret