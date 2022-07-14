; global __cmos_read

; __cmos_read:
;     push rbp
;     mov rbp, rsp
;     mov qword [rbp-24], rdi
;     cmp qword [rbp-24], 0
;     je __cmos_read02
;     mov byte [rbp-1], 0
;     mov byte [rbp-2], 0
;     jmp __cmos_read00
;     ret

; __cmos_read01:
;     movzx edx, byte [rbp-1]
;     mov rax, qword [rbp-24]
;     add rax, rdx
;     movzx eax, byte [rax]
;     mov byte [rbp-2], al

;     cli
;     push al
;     movzx eax, byte [rbp-1]
;     mov al, eax
;     out 0x70, al


;     add eax, 1
;     mov byte [rbp-1], al
; __cmos_read00:
;     movzx eax, byte [rbp - 2]
;     test al, al
;     jns __cmos_read01
;     jmp __cmos_read02
; __cmos_read02:
;     pop rbp
;     ret