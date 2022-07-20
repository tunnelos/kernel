#include "../include/rtc.h"
#include "../include/idt.h"
#include "../include/nmi.h"

void __rtc_init() {
    __cli();
    __nmi_disable();
    outb(RTC_REGISTER_B_OUT, 0x8B);
    char p = inb(RTC_REGISTER_B_IN);
    outb(RTC_REGISTER_B_OUT, 0X8B);
    outb(RTC_REGISTER_B_IN, p | 0x40);
    __sti();
    outb(RTC_REGISTER_B_OUT, 0x0C);
    inb(RTC_REGISTER_B_IN);
}