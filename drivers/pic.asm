global __pic_disable

__pic_disable:
    mov al, 0xFF
    out 0xA1, al
    out 0x21, al
    ret