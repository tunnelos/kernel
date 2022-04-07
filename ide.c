#include "./include/ide.h"

ide_channel_reg_t __ide_channels[2];
uint8_t __ide_buffer[2048] = {0};
volatile uint8_t __ide_irq_trigger;
uint8_t __ide_atapi_packet[12];
ide_dev_t __ide_devices[4];

void __ide_init(uint32_t *bars) {

}

uint8_t __ide_read(uint8_t channel, uint8_t reg) {
    uint8_t res = 0;
    ide_channel_reg_t cc = __ide_channels[channel];
    if(reg > 0x07 && reg < 0x0C) __ide_write(channel, ATA_REG_CONTROL, 0x80 | __ide_channels[channel].no_int);
    if(reg < 0x08) res = inb(cc.base + reg);
    else if (reg < 0x0C) res = inb(cc.base + reg - 6);
    else if (reg < 0x0E) res = inb(cc.ctrl + reg - 0x0A);
    else if (reg < 0x16) res = inb(cc.bmide + reg - 0x0E);
    if (reg > 0x07 && reg < 0x0C) __ide_write(channel, ATA_REG_CONTROL, cc.no_int);
    return res;
}

void __ide_write(uint8_t channel, uint8_t reg, uint8_t data) {
    ide_channel_reg_t cc = __ide_channels[channel];
    if(reg > 0x07 && reg < 0x0C) __ide_write(channel, ATA_REG_CONTROL, 0x80 | cc.no_int);
    if(reg < 0x08) outb(cc.base + reg, data);
    else if (reg < 0x0C) outb(cc.base + reg - 0x06, data);
    else if (reg < 0x0E) outb(cc.ctrl + reg - 0x0A, data);
    else if (reg < 0x16) outb(cc.bmide + reg - 0x0E, data);
    if (reg > 0x07 && reg < 0x0C) __ide_write(channel, ATA_REG_CONTROL, cc.no_int);
}

void __ide_read_buffer(uint8_t channel, uint8_t reg, uint32_t *buffer, uint32_t quads) {
    
}