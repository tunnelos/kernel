global __avx_init

__avx_init:
    push rax
    push rcx
    push rdx
    xor rcx, rcx
    xgetbv 
    or eax, 7
    xsetbv
    pop rdx
    pop rcx
    pop rax
    ret