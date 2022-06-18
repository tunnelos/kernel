#include "../include/nmi.h"
#include "../include/rtc.h"
#include "../include/tools.h"

void __nmi_init() {
    outb(RTC_REGISTER_B_OUT, inb(RTC_REGISTER_B_OUT) & 0x7F);
    inb(RTC_REGISTER_B_IN);
    return;
}
void __nmi_disable() {
    outb(RTC_REGISTER_B_OUT, inb(RTC_REGISTER_B_OUT) | 0x80);
    inb(RTC_REGISTER_B_IN);
    return;
}