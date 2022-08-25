#include "../include/tunnel.h"
#include "../include/idt.h"

tunnelos_sysinfo_t tunnelos_sysinfo;

//const char *compile_timestamp = __TIMESTAMP__;

void __tunnel_reboot() {
    uint8_t t;
    __cli();
    while(check_flag(t, 1) != 0) {
        t = inb(0x64);
        if(check_flag(t, 0) != 0) inb(0x60);
    }
    outb(0x64, 0xFE);
    hlt:
    asm volatile("hlt");
    jmp hlt;
}
void __tunnel_shutdown() {
    outw(0xB004, 0x2000);
    outw(0x604, 0x2000);
    outw(0x4004, 0x3400);
}