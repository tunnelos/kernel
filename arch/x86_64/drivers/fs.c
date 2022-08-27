#include "../include/fs.h"
#include "../include/ide.h"
#include "../include/mm.h"
#include "../include/assert.h"

bool __fs_readFATCheck(uint8_t drive) {
    ide_rw_t ideAction;
    uint8_t *buffer = (uint8_t *)malloc(512);
    ideAction.buffer = (int)buffer;
    ideAction.drive = drive;
    ideAction.lba = 0;
    ideAction.rw = false;
    ideAction.sectors = 1;
    ideAction.selector = 0;
    __ide_get_access(ideAction);
    if(buffer[0] != 0xEB && buffer[2] != 0xFE) {
        free(buffer);
        return false;
    }
    free(buffer);
    return true;
}
enum FATType __fs_getFATType(uint8_t drive) {
    ide_rw_t ideAction;
    uint8_t *buffer = (uint8_t *)malloc(512);
    ideAction.buffer = (int)buffer;
    ideAction.drive = drive;
    ideAction.lba = 0;
    ideAction.rw = false;
    ideAction.sectors = 1;
    ideAction.selector = 0;
    __ide_get_access(ideAction);
    bpb_t *bpb = (bpb_t *)buffer;
    if(bpb->sectors == 0) {
        free(buffer);
        return FAT32;
    } else {
        int fatSize = (bpb->fat16Table == 0) ? ((fat32_t *)buffer)->tableSize : bpb->fat16Table;
        int rootSectors = ((bpb->rootDirEntries * 32) + (bpb->bps - 1)) / bpb->bps;
        int dataSectors = bpb->sectors - (bpb->reservedSectors + (bpb->fat16Table * fatSize) + rootSectors);
        int totalClusters = dataSectors / bpb->spc;
        if(totalClusters < 4085) {
            free(buffer);
            return FAT12;
        }
        if(totalClusters < 65525) {
            free(buffer);
            return FAT16;
        } else {
            free(buffer);
            return FAT32;
        }
    }
    // return true;
}