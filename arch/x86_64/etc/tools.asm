global __cli
global __sti
global __hlt
global __read_error_code
global __get_stack
global __pit_eoi
global __tools_int_test

__cli:
    cli
    ret
__sti:
    sti
    ret
__hlt:
    hlt
__read_error_code:
    mov eax, dword [rsp-4]
    ret
__get_stack:
    mov rax, rsp
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