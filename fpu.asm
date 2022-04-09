global __fpu_check
global __fpu_cr0_ts
global __fpu_cr0_em

__fpu_cr0_ts: db 8
__fpu_cr0_em: db 4
testword: dw 0x55AA

__fpu_check:
    mov rdx, cr0
    and rdx, (-1) - 12
    mov cr0, rdx
    fninit
    fnstsw [testword]
    cmp dword [testword], 0
    ; WIP
    jne __fpu_check_fail
    jmp __fpu_check_complete
__fpu_check_fail:
    ret
__fpu_check_complete:
    ret