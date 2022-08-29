#pragma once

#include "./stdint.h"
#include "./tools.h"
#include "./serial.h"

#define ATA_SR_BSY  0x80 // Busy
#define ATA_SR_DRDY 0x40 // Drive ready
#define ATA_SR_DF   0x20 // Drive write fault
#define ATA_SR_DSC  0x10 // Drive seek complete
#define ATA_SR_DRQ  0x08 // Data request ready
#define ATA_SR_CORR 0x04 // Corrected data
#define ATA_SR_IDX  0x02 // Index
#define ATA_SR_ERR  0x01 // Error

#define ATA_ER_BBK   0x80 // Bad block
#define ATA_ER_UNC   0x40 // Uncorrectable data
#define ATA_ER_MC    0x20 // Media changed
#define ATA_ER_IDNF  0x10 // ID mark not found
#define ATA_ER_MCR   0x08 // Media change request
#define ATA_ER_ABRT  0x04 // Command aborted
#define ATA_ER_TK0NF 0x02 // Track 0 not found
#define ATA_ER_AMNF  0x01 // No address mark

#define ATA_CMD_READ_PIO        0x20
#define ATA_CMD_READ_PIO_EXT    0x24
#define ATA_CMD_READ_DMA        0xC8
#define ATA_CMD_READ_DMA_EXT    0x25
#define ATA_CMD_WRITE_PIO       0x30
#define ATA_CMD_WRITE_PIO_EXT   0x34
#define ATA_CMD_WRITE_DMA       0xCA
#define ATA_CMD_WRITE_DMA_EXT   0x35
#define ATA_CMD_CACHE_FLUSH     0xE7
#define ATA_CMD_CACHE_FLUSH_EXT 0xEA
#define ATA_CMD_PACKET          0xA0
#define ATA_CMD_IDENTIFY_PACKET 0xA1
#define ATA_CMD_IDENTIFY        0xEC

#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

#define IDE_ATA    0x00
#define IDE_ATAPI  0x01
#define ATA_MASTER 0x00
#define ATA_SLAVE  0x01

#define ATAPI_CMD_READ  0xA8
#define ATAPI_CMD_EJECT 0x1B

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Channels:
#define ATA_PRIMARY   0x00
#define ATA_SECONDARY 0x01
 
// Directions:
#define ATA_READ  0x00
#define ATA_WRITE 0x01

#define IDE_LBA48 2
#define IDE_LBA28 1
#define IDE_CHS 0

#define IDE_HAS_LBA 0x200

#define ATA_DMA_SUPPORTED 1
#define ATA_DMA_UNSUPPORTED 0

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t base;
    uint16_t ctrl;
    uint16_t bmide;
    uint8_t no_int;
} ide_channel_reg_t;
typedef struct {
    bool connected;
    uint8_t channel;
    uint8_t drive;
    uint16_t type;
    uint16_t signature;
    uint16_t capabilities;
    uint32_t cmd_set;
    uint8_t addressing_mode;
    // In kilobits;
    uint32_t size;
    uint8_t model[41];
} ide_dev_t;
typedef struct {
    //0 - read | 1 - write
    bool rw;
    uint8_t drive;
    uint32_t lba;
    uint8_t sectors;
    uint16_t selector;
    uint32_t buffer;
} ide_rw_t;

extern ide_channel_reg_t __ide_channels[2];
extern uint8_t __ide_buffer[2048];
extern volatile uint8_t __ide_irq_trigger;
extern uint8_t __ide_atapi_packet[12];
extern ide_dev_t __ide_devices[4];
extern const char *__ide_error_table[11];
extern const char *__ide_drive_table1[2];
extern const char *__ide_drive_table2[2];

void __ide_init(uint32_t *bars);
uint8_t __ide_read(uint8_t channel, uint8_t reg);
void __ide_write(uint8_t channel, uint8_t reg, uint8_t data);
void __ide_read_buffer(uint8_t channel, uint8_t reg, uint32_t *buffer, uint32_t quads);
uint8_t __ide_polling(uint8_t channel, bool advanced);
void __ide_error(uint32_t drive, uint8_t err);

void __ide_get_access(ide_rw_t data);

#ifdef __cplusplus
}
#endif