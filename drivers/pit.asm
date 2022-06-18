global __pit_init
global __pit_system_timer_fractions
global __pit_system_timer_ms
global __pit_IRQ0_fractions
global __pit_IRQ0_ms
global __pit_IRQ0_frequency
global __pit_reload_value
extern __pit_eoi
extern __pit_event_timer

section .bss
__pit_system_timer_fractions:  resd 1          ; Fractions of 1 ms since timer initialized
__pit_system_timer_ms:         resd 1          ; Number of whole ms since timer initialized
__pit_IRQ0_fractions:          resd 1          ; Fractions of 1 ms between IRQs
__pit_IRQ0_ms:                 resd 1          ; Number of whole ms between IRQs
__pit_IRQ0_frequency:          resd 1          ; Actual frequency of PIT
__pit_reload_value:            resw 1          ; Current PIT reload value
section .text

__pit_interrupt:
    push rax
    push rbx
    call __pit_event_timer
    call __pit_eoi
    pop rbx
    pop rax
    iretd
__pit_init:
    cli
    push rbx
    mov eax, 0x10000
    cmp ebx, 18
    jbe __pit_init_000
    mov eax, 1
    cmp ebx, 1193181
    jae __pit_init_000
    mov eax, 3579545
    mov edx, 0
    div ebx
    cmp edx, 3579545 / 2
    jb __pit_init_001
    inc eax
__pit_init_001:
    mov ebx, 3
    mov edx, 0
    div ebx
    cmp edx, 3 / 2
    jb __pit_init_002
    inc eax
__pit_init_002:
__pit_init_000:
    push rax
    mov [__pit_reload_value], ax
    mov ebx, eax
    mov eax, 3579545
    mov edx, 0
    div ebx
    cmp edx, 3579545 / 2
    jb __pit_init_003
__pit_init_003:
    mov ebx, 3
    mov edx, 0
    div ebx
    cmp edx, 3 / 2
    jb __pit_init_004
    inc eax
__pit_init_004:
    mov [__pit_IRQ0_frequency], eax
    pop rbx
    mov eax, 0xDBB3A062
    mul ebx
    shrd eax, edx, 10
    shr edx, 10
    mov [__pit_IRQ0_ms], edx
    mov [__pit_IRQ0_fractions], eax
    cli
    mov al, 00110100b
    out 0x43, al
    mov ax, [__pit_reload_value]
    out 0x40, al
    mov al, ah
    out 0x40, al
    pop rax
    sti
    ret