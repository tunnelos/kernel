global __cpuid_check_sse
global __cpuid_check_avx
global __cpuid_check_avx2
global __cpuid_check_avx512

__cpuid_check_sse_l1:
    mov eax, DWORD [rbp-4]
    pop rbp
    ret
__cpuid_check_sse:
    push rbp
    mov rbp, rsp
    mov DWORD [rbp-4], 0
    mov eax, 0x1
    cpuid
    test edx, 1 << 25
    jz __cpuid_check_sse_l1
    mov DWORD [rbp-4], 1
    mov eax, DWORD [rbp-4]
    pop rbp
    ret
__cpuid_check_avx:
    push rbp
    mov rbp, rsp
    mov DWORD [rbp-4], 0
    mov eax, 0x1
    cpuid
    test ecx, 1 << 29
    jz __cpuid_check_sse_l1
    mov DWORD [rbp-4], 1
    mov eax, DWORD [rbp-4]
    pop rbp
    ret
__cpuid_check_avx2:
    push rbp
    mov rbp, rsp
    mov DWORD [rbp-4], 0
    mov eax, 0x7
    cpuid
    test edx, 1 << 26
    jz __cpuid_check_sse_l1
    mov DWORD [rbp-4], 1
    mov eax, DWORD [rbp-4]
    pop rbp
    ret
__cpuid_check_avx512:
    push rbp
    mov rbp, rsp
    mov DWORD [rbp-4], 0
    mov eax, 0x0D
    cpuid
    test eax, 1 << 5
    jz __cpuid_check_sse_l1
    mov DWORD [rbp-4], 1
    mov eax, DWORD [rbp-4]
    pop rbp
    ret
