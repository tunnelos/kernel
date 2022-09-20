global __pit_init
global __pit_system_timer_fractions
global __pit_system_timer_ms
global __pit_IRQ0_fractions
global __pit_IRQ0_ms
global __pit_IRQ0_frequency
global __pit_reload_value
extern __pit_eoi
extern __pit_event_timer
extern __serial_write_raw
extern __serial_write_char

section .data
__pit_system_timer_fractions:  dd 1        ; Fractions of 1 ms since timer initialized
__pit_system_timer_ms:       dd 1        ; Number of whole ms since timer initialized
__pit_IRQ0_fractions:        dd 1        ; Fractions of 1 ms between IRQs
__pit_IRQ0_ms:             dd 1        ; Number of whole ms between IRQs
__pit_IRQ0_frequency:        dd 1        ; Actual frequency of PIT
__pit_reload_value:         dd 1        ; Current PIT reload value
__pit_log0                db "PIT Init"  
__pit_log1                db "Calculating frequency"
__pit_log2                db "Starting up PIT Channel"
__pit_log3                db "Frequency check"
__pit_log4                db "Reload value"
section .text

__pit_init:
   mov ebx, 100000
   mov rbp, rsp
   mov rsi, 9
   mov rdi, __pit_log0
   mov rdi, 10
   mov eax, 0

   pushaq
 
   ; Do some checking
 
   mov eax,0x10000               ;eax = reload value for slowest possible frequency (65536)
   cmp ebx,18                  ;Is the requested frequency too low?
   jbe .gotReloadValue            ; yes, use slowest possible frequency
 
   mov eax,1                   ;ax = reload value for fastest possible frequency (1)
   cmp ebx,1193181               ;Is the requested frequency too high?
   jae .gotReloadValue            ; yes, use fastest possible frequency
 
   ; Calculate the reload value
   
   mov rsi, 9
   mov rdi, __pit_log1
   mov rdi, 10
   mov eax, 0
   mov eax,3579545
   mov edx,0                   ;edx:eax = 3579545
   div ebx                     ;eax = 3579545 / frequency, edx = remainder
   cmp edx,3579545 / 2            ;Is the remainder more than half?
   jb .l1                     ; no, round down
   inc eax                     ; yes, round up
 .l1:
   mov rsi, 9
   mov rdi, __pit_log3
   mov rdi, 10
   mov eax, 0
   mov ebx,3
   mov edx,0                   ;edx:eax = 3579545 * 256 / frequency
   div ebx                     ;eax = (3579545 * 256 / 3 * 256) / frequency
   cmp edx,3 / 2                ;Is the remainder more than half?
   jb .l2                     ; no, round down
   inc eax                     ; yes, round up
 .l2:
 
 
 ; Store the reload value and calculate the actual frequency
 
 .gotReloadValue:
   push rax                    ;Store reload_value for later
   mov [__pit_reload_value],ax       ;Store the reload value for later
   mov ebx,eax                  ;ebx = reload value
   mov rsi, 9
   mov rdi, __pit_log4
   mov rdi, 10
   mov eax,3579545
   mov edx,0                   ;edx:eax = 3579545
   div ebx                     ;eax = 3579545 / reload_value, edx = remainder
   cmp edx,3579545 / 2            ;Is the remainder more than half?
   jb .l3                     ; no, round down
   inc eax                     ; yes, round up
 .l3:
   mov rsi, 9
   mov rdi, __pit_log3
   mov rdi, 10
   mov eax, 0
   mov ebx,3
   mov edx,0                   ;edx:eax = 3579545 / reload_value
   div ebx                     ;eax = (3579545 / 3) / frequency
   cmp edx,3 / 2                ;Is the remainder more than half?
   jb .l4                     ; no, round down
   inc eax                     ; yes, round up
 .l4:
   mov [__pit_IRQ0_frequency],eax        ;Store the actual frequency for displaying later
 
 
 ; Calculate the amount of time between IRQs in 32.32 fixed point
 ;
 ; Note: The basic formula is:
 ;         time in ms = reload_value / (3579545 / 3) * 1000
 ;      This can be rearranged in the following way:
 ;         time in ms = reload_value * 3000 / 3579545
 ;         time in ms = reload_value * 3000 / 3579545 * (2^42)/(2^42)
 ;         time in ms = reload_value * 3000 * (2^42) / 3579545 / (2^42)
 ;         time in ms * 2^32 = reload_value * 3000 * (2^42) / 3579545 / (2^42) * (2^32)
 ;         time in ms * 2^32 = reload_value * 3000 * (2^42) / 3579545 / (2^10)
   
   mov rsi, 9
   mov rdi, __pit_log4
   mov rdi, 10
   mov eax, 0
   pop rbx                     ;ebx = reload_value
   mov eax,0xDBB3A062            ;eax = 3000 * (2^42) / 3579545
   mul ebx                     ;edx:eax = reload_value * 3000 * (2^42) / 3579545
   shrd eax,edx,10
   shr edx,10                  ;edx:eax = reload_value * 3000 * (2^42) / 3579545 / (2^10)
 
   mov [__pit_IRQ0_ms],edx             ;Set whole ms between IRQs
   mov [__pit_IRQ0_fractions],eax        ;Set fractions of 1 ms between IRQs
 
 
 ; Program the PIT channel
   
   mov rsi, 9
   mov rdi, __pit_log2
   mov rdi, 10
   mov eax, 0
   pushfq
   cli                        ;Disabled interrupts (just in case)
 
   mov al,00110100b              ;channel 0, lobyte/hibyte, rate generator
   out 0x43, al
 
   mov ax,[__pit_reload_value]       ;ax = 16 bit reload value
   out 0x40,al                  ;Set low byte of PIT reload value
   mov al,ah                   ;ax = high 8 bits of reload value
   out 0x40,al                  ;Set high byte of PIT reload value
 
   popfq
 
   popaq
   ret