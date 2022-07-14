#include "../include/cmos.h"
#include "../include/tools.h"
#include "../include/idt.h"
#include "../include/rtc.h"

unsigned char __cmos_data[128];
cmos_t __cmos_time;
bool __cmos_firstInt = true;

int __cmos_getUpdate() {
    outb(RTC_REGISTER_B_OUT, 0x0A);
    return inb(RTC_REGISTER_B_IN) & 0x80;
}
uint8_t __cmos_getRTCreg(uint8_t reg) {
    outb(RTC_REGISTER_B_OUT, reg);
    return inb(RTC_REGISTER_B_IN);
}
void __cmos_getRTC() {
    cmos_t *ct = &__cmos_time;
    while(__cmos_getUpdate());
    ct->second  = __cmos_getRTCreg(0);
    ct->minute  = __cmos_getRTCreg(2);
    ct->hour    = __cmos_getRTCreg(4);
    ct->day     = __cmos_getRTCreg(7);
    ct->month   = __cmos_getRTCreg(8);
    ct->year    = 0;
    ct->century = 21; 

    uint8_t rb = __cmos_getRTCreg(0x0B);
    if (!(rb & 0x04)) {
        ct->second = (ct->second & 0x0F) + ((ct->second / 16) * 10);
        ct->minute = (ct->minute & 0x0F) + ((ct->minute / 16) * 10);
        ct->hour = ((ct->hour & 0x0F) + (((ct->hour & 0x70) / 16) * 10)) | (ct->hour & 0x80);
        ct->day = (ct->day & 0x0F) + ((ct->day / 16) * 10);
        ct->month = (ct->month & 0x0F) + ((ct->month / 16) * 10);
        ct->year = (ct->year & 0x0F) + ((ct->year / 16) * 10);
    }
    if (!(rb & 0x02) && (ct->hour & 0x80)) ct->hour = ((ct->hour & 0x7F) + 12) % 24;
    ct->year += (CMOS_YEAR / 100) * 100;
    if(ct->year < CMOS_YEAR) ct->year += 100;

}
void __cmos_read(unsigned char *data_array) {
    unsigned char i = 0, v = 0;
    while(i < 128) {
        __cli();
        outb(0x70, i);
        wait(100);
        v = inb(0x71);
        __sti();
        data_array[i] = v;
        i++;
    }
    return;
}