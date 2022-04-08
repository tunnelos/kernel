#include "./include/ide.h"

ide_channel_reg_t __ide_channels[2];
uint8_t __ide_buffer[2048] = {0};
volatile uint8_t __ide_irq_trigger;
uint8_t __ide_atapi_packet[12];
ide_dev_t __ide_devices[4];
const char *__ide_error_table[11] = {
    "No address mark found", "No media or media error",
    "Command aborted", "No media or media error",
    "ID mark not found", "No media or media error",
    "Uncorrectable data error", "Bad sectors",
    "Reading nothing", "Read-only data",
    "Device fault"
};
const char *__ide_drive_table1[2] = {
    "Primary", "Secondary"
};
const char *__ide_drive_table2[2] = {
    "Master", "Slave"
};

void __ide_init(uint32_t *bars) {
    __serial_write_fmt("CPU %d -> tos > Searching for IDE devices...\r\n", __tools_get_cpu() - 1);

    int j = 0, k = 0, count = 0;

    __ide_channels[ATA_PRIMARY].base = (bars[0] & 0xFFFFFFFC) + 0x1F0 * (!bars[0]);
    __ide_channels[ATA_PRIMARY].ctrl = (bars[1] & 0xFFFFFFFC) + 0x3F6 * (!bars[1]);
    __ide_channels[ATA_SECONDARY].base = (bars[2] & 0xFFFFFFFC) + 0x170 * (!bars[2]);
    __ide_channels[ATA_SECONDARY].ctrl = (bars[3] & 0xFFFFFFFC) + 0x376 * (!bars[3]);
    __ide_channels[ATA_PRIMARY].bmide = (bars[4] & 0xFFFFFFFC) + 0;
    __ide_channels[ATA_SECONDARY].bmide = (bars[4] & 0xFFFFFFFC) + 8;

    __ide_write(ATA_PRIMARY, ATA_REG_CONTROL, 2);
    __ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);
    while(k < 2) {
        while(j < 2) {
            uint8_t err = 0, type = IDE_ATA, status = 0;
            __ide_devices[count].connected = false;

            __ide_write(k, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
            wait(1);
            __ide_write(k, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
            wait(1);
            if(__ide_read(k, ATA_REG_STATUS) != 0) {
                __serial_write_fmt("CPU %d -> tos > Found evidence of IDE device.\r\n", __tools_get_cpu() - 1);
            } else {
                __serial_write_fmt("CPU %d -> tos > IDE device not found. Skipping.\r\n", __tools_get_cpu() - 1);
                break;
            }
            bool ssksk = false;
            while(!ssksk) {
                status = __ide_read(k, ATA_REG_STATUS);
                if ((status & ATA_SR_ERR)) {
                    err = 1; 
                    __ide_error(j, err);
                    __serial_write_fmt("CPU %d -> tos > Invalid device. Skipping drive.\r\n", __tools_get_cpu() - 1);
                    break;
                } else {
                    ssksk = true;
                    break;
                }
                if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) ssksk = true;
                
            }
            if (err != 0) {
                uint8_t cl = __ide_read(k, ATA_REG_LBA1);
                uint8_t ch = __ide_read(k, ATA_REG_LBA2);

                if(cl == 0x14 && ch == 0xEB) {
                    __serial_write_fmt("CPU %d -> tos > ATAPI device found\r\n", __tools_get_cpu() - 1);
                    type = IDE_ATAPI;
                }
                else if (cl == 0x69 && ch == 0x96) {
                    __serial_write_fmt("CPU %d -> tos > ATAPI device found\r\n", __tools_get_cpu() - 1);
                    type = IDE_ATAPI;
                }
                else {
                    __serial_write_fmt("CPU %d -> tos > Unknown device found. Skipping it.\r\n", __tools_get_cpu() - 1);
                    break;
                }

                __ide_write(k, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
                wait(1);
            }

            __ide_read_buffer(k, ATA_REG_DATA, (uint32_t *)__ide_buffer, 128);
            
            __ide_devices[count].connected = true;
            __ide_devices[count].type = type;
            __ide_devices[count].channel = k;
            __ide_devices[count].drive = j;
            __ide_devices[count].signature = *((uint16_t *)(__ide_buffer + ATA_IDENT_DEVICETYPE));
            __ide_devices[count].capabilities = *((uint16_t *)(__ide_buffer + ATA_IDENT_CAPABILITIES));
            __ide_devices[count].cmd_set = *((uint32_t *)(__ide_buffer + ATA_IDENT_COMMANDSETS));
            if (__ide_devices[count].cmd_set & (1 << 26)) {
                __ide_devices[count].addressing_mode = 48;
                __ide_devices[count].size = *((uint32_t *)(__ide_buffer + ATA_IDENT_MAX_LBA_EXT));
            } else {
                __ide_devices[count].addressing_mode = 28;
                __ide_devices[count].size = *((uint32_t *)(__ide_buffer + ATA_IDENT_MAX_LBA));
            }
            if(__ide_devices[count].size == 0) {
                __serial_write_fmt("CPU %d -> tos > Found garbade data. Skipping it.\r\n", __tools_get_cpu() - 1);
                __ide_devices[count].connected = false;
                break;
            } else {
                __serial_write_fmt("CPU %d -> tos > Found connected IDE device (%s %s)\r\n", __tools_get_cpu() - 1, __ide_drive_table1[__ide_devices[count].channel], __ide_drive_table2[__ide_devices[count].drive]);
                __serial_write_fmt("CPU %d -> tos > Connected IDE size: %dMb with %d-bit addressing\r\n", __tools_get_cpu() - 1, __ide_devices[count].size / 1024 / 2, __ide_devices[count].addressing_mode);
            }

            for (uint8_t i = 0; i < 40; i++) __ide_devices[count].model[i] = __ide_buffer[ATA_IDENT_MODEL + i];
            __ide_devices[count].model[40] = 0;
            __serial_write_fmt("CPU %d -> tos > Connected IDE model: %d\r\n", __tools_get_cpu() - 1, __ide_devices[count].model);

            j++;
            count++;
        }
        j = 0;
        k++;
    }
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
    ide_channel_reg_t cc = __ide_channels[channel];
    if(reg > 0x08 && reg < 0x0C) __ide_write(channel, ATA_REG_CONTROL, 0x80 | cc.no_int);
    __ide_pushw();
    if (reg < 0x08) insl(cc.base  + reg - 0x00, buffer, quads);
    else if (reg < 0x0C) insl(cc.base  + reg - 0x06, buffer, quads);
    else if (reg < 0x0E) insl(cc.ctrl  + reg - 0x0A, buffer, quads);
    else if (reg < 0x16) insl(cc.bmide + reg - 0x0E, buffer, quads);
    __ide_popw();
    if (reg > 0x07 && reg < 0x0C) __ide_write(channel, ATA_REG_CONTROL, cc.no_int);
    return;
}

uint8_t __ide_polling(uint8_t channel, bool advanced) {
    for(uint8_t i = 0; i < 4; i++) __ide_read(channel, ATA_REG_ALTSTATUS);
    while(__ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY);
    if(advanced) {
        uint8_t state = __ide_read(channel, ATA_REG_STATUS);
        if (state & ATA_SR_ERR) return 2;
        if (state & ATA_SR_DF) return 1;
        if (!(state & ATA_SR_DRQ)) return 3;
    }
    return 0;
}

void __ide_error(uint32_t drive, uint8_t err) {
    if(!err) return;
    
    __serial_write_fmt("CPU %d -> tos > IDE error report.\r\n", __tools_get_cpu() - 1);
    uint8_t jj = 0;
    if(err == 1) {
        jj = 10;
    } else if (err == 2) {
        uint8_t st = __ide_read(__ide_devices[drive].channel, ATA_REG_ERROR);
        if(st & ATA_ER_TK0NF) jj = 1;
        if(st & ATA_ER_ABRT) jj = 2;
        if(st & ATA_ER_MCR) jj = 3;
        if(st & ATA_ER_IDNF) jj = 4;
        if(st & ATA_ER_MC) jj = 5;
        if(st & ATA_ER_UNC) jj = 6;
        if(st & ATA_ER_BBK) jj = 7;
    } else if (err == 3) {
        jj = 8;
    } else if (err == 4) {
        jj = 9;
    }
    __serial_write_fmt("CPU %d -> tos > * Reported Error: %s\r\n", __tools_get_cpu() - 1, __ide_error_table[jj]);
    ide_dev_t id = __ide_devices[drive];
    __serial_write_fmt("CPU %d -> tos > * Reported Device: %s %s [%s]\r\n", __tools_get_cpu() - 1, __ide_drive_table1[id.channel], __ide_drive_table2[id.drive], id.model);

    return;
}

void __ide_get_access(ide_rw_t data) {
    uint8_t lba_mode, dma, cmd;
    uint8_t lba_io[6];
    uint32_t channel = __ide_devices[data.drive].channel;
    uint32_t slavebit = __ide_devices[data.drive].drive;
    uint32_t bus = __ide_channels[channel].base;
    uint32_t words = 256;
    uint16_t cyl, i = 0;
    uint8_t head, sect, err;

    __ide_write(channel, ATA_REG_CONTROL, __ide_channels[channel].no_int = (__ide_irq_trigger = 0) + 2);

    if(data.lba >= 0x10000000) {
        lba_mode = IDE_LBA48;
        lba_io[0] = (data.lba & 0x000000FF) >> 0;
        lba_io[1] = (data.lba & 0x0000FF00) >> 8;
        lba_io[2] = (data.lba & 0x00FF0000) >> 16;
        lba_io[3] = (data.lba & 0xFF000000) >> 24;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = 0;
    } else if (__ide_devices[data.drive].capabilities & IDE_HAS_LBA) {
        lba_mode = IDE_LBA28;
        lba_io[0] = (data.lba & 0x000000FF) >> 0;
        lba_io[1] = (data.lba & 0x0000FF00) >> 8;
        lba_io[2] = (data.lba & 0x00FF0000) >> 16;
        head = (data.lba & 0xF000000) >> 24;
    } else {
        lba_mode = IDE_CHS;
        sect = (data.lba % 63) + 1;
        cyl = (data.lba + 1 - sect) / (16 * 63);
        lba_io[0] = sect;
        lba_io[1] = (cyl >> 0) & 0xFF;
        lba_io[2] = (cyl >> 8) & 0xFF;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = (data.lba + 1 - sect) % (16 * 63) / (63);
    }
    dma = 0;
    while(__ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY);
    if(!lba_mode) __ide_write(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit << 4) | head);
    else __ide_write(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit << 4) | head);

    if(lba_mode == IDE_LBA48) {
        __ide_write(channel, ATA_REG_SECCOUNT1, 0);
        __ide_write(channel, ATA_REG_LBA3, lba_io[3]);
        __ide_write(channel, ATA_REG_LBA4, lba_io[4]);
        __ide_write(channel, ATA_REG_LBA5, lba_io[5]);
    }
    __ide_write(channel, ATA_REG_SECCOUNT0, data.sectors);
    __ide_write(channel, ATA_REG_LBA0, lba_io[0]);
    __ide_write(channel, ATA_REG_LBA1, lba_io[1]);
    __ide_write(channel, ATA_REG_LBA2, lba_io[2]);

         if (lba_mode == IDE_CHS && dma == ATA_DMA_UNSUPPORTED && data.rw == ATA_READ) cmd = ATA_CMD_READ_PIO;
    else if (lba_mode == IDE_LBA28 && dma == ATA_DMA_UNSUPPORTED && data.rw == ATA_READ) cmd = ATA_CMD_READ_PIO;   
    else if (lba_mode == IDE_LBA48 && dma == ATA_DMA_UNSUPPORTED && data.rw == ATA_READ) cmd = ATA_CMD_READ_PIO_EXT;   
    else if (lba_mode == IDE_CHS && dma == ATA_DMA_SUPPORTED && data.rw == ATA_READ) cmd = ATA_CMD_READ_DMA;
    else if (lba_mode == IDE_LBA28 && dma == ATA_DMA_SUPPORTED && data.rw == ATA_READ) cmd = ATA_CMD_READ_DMA;
    else if (lba_mode == IDE_LBA48 && dma == ATA_DMA_SUPPORTED && data.rw == ATA_READ) cmd = ATA_CMD_READ_DMA_EXT;
    else if (lba_mode == IDE_CHS && dma == ATA_DMA_UNSUPPORTED && data.rw == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO;
    else if (lba_mode == IDE_LBA28 && dma == ATA_DMA_UNSUPPORTED && data.rw == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO;
    else if (lba_mode == IDE_LBA48 && dma == ATA_DMA_UNSUPPORTED && data.rw == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO_EXT;
    else if (lba_mode == IDE_CHS && dma == ATA_DMA_SUPPORTED && data.rw == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA;
    else if (lba_mode == IDE_LBA28 && dma == ATA_DMA_SUPPORTED && data.rw == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA;
    else if (lba_mode == IDE_LBA48 && dma == ATA_DMA_SUPPORTED && data.rw == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA_EXT;
    __ide_write(channel, ATA_REG_COMMAND, cmd);

    if (dma) {
        if(data.rw == ATA_READ) {
            //read
        } else {
            //write
        }
    } else {
        if(data.rw == ATA_READ) {
            //read
            while(i < data.sectors) {
                err = __ide_polling(channel, true);
                if(err) return;
                asm volatile("mov %%ax, %%es" : : "a"(data.selector));
                asm volatile("rep insw" : : "c"(words), "d"(bus), "D"(data.buffer));
                data.buffer += (words * 2);
                i++;
            }
        } else {
            //write
            while(i < data.sectors) {
                __ide_polling(channel, 0);
                asm volatile("mov %%ax, %%ds" : : "a"(data.selector));
                asm volatile("rep outsw" : : "c"(words), "d"(bus), "S"(data.buffer));
                data.buffer += (words * 2);
                i++;
            }
            __ide_write(channel, ATA_REG_COMMAND, (char []) { ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH_EXT }[lba_mode]);
            __ide_polling(channel, 0);
        }
    }
    return;
}