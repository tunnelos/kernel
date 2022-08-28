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
}

void *__fs_makeSectorAction(int sID, int sSize, void *buffer, enum SectorAction action, uint8_t drive) {
    assert(buffer);

    ide_rw_t ideAction;
    ideAction.buffer = (int)buffer;
    ideAction.drive = drive;
    ideAction.lba = sID;
    ideAction.rw = (action == Read) ? false : true;
    ideAction.sectors = sSize;
    ideAction.selector = 0;
    __ide_get_access(ideAction);

    return buffer;
}

bool __fs_tunnelFindFS(uint8_t drive) {
    void *buffer = malloc(512);
    assert(buffer);

    __fs_makeSectorAction(0, 1, buffer, Read, drive);
    tunnelfs_bootsector_t *boot = (tunnelfs_bootsector_t *)buffer;

    if(strncmp(boot->signature, "TUNNELFS", 8) == 0) {
        free(buffer);
        return true;
    }
    free(buffer);
    return false;
}
tunnelfs_t __fs_tunnelCreateFS(int *percentage, uint8_t drive) {
    if(__ide_devices[drive].connected) {
        tunnelfs_t res;

        void *buffer = calloc(512);

        int i = 0;
        int sectors = __ide_devices[drive].size / 2 * 1024 / 512;

        float p;
        while(i < sectors) {
            __fs_makeSectorAction(i, 1, buffer, Write, drive);
            if(percentage) {
                p = ((float)i / (float)sectors) * (float)100;
                *percentage = (int)p;
            }
            i++;
        }
        *percentage = 99;

        i = 0;
        tunnelfs_bootsector_t *boot = (tunnelfs_bootsector_t *)buffer;

        memcpy(boot->signature, "TUNNELFS", 8);
        boot->tableSector = 1;
        boot->tableSize = sizeof(tunnelfs_table_t) / 512;
        boot->sectorsAvaliable = sectors - (boot->tableSize + 1);
        boot->lastFreeSectorID = 512;
        boot->lastFreeSectorSize = boot->sectorsAvaliable;
        boot->totalSectors = sectors;
        boot->filesCreated = 0;
        boot->revision = TUNNELFS_1;

        res.bootsector = (tunnelfs_bootsector_t *)buffer;
        res.table = (tunnelfs_table_t *)malloc(boot->tableSize * 512);
        res.drive = drive;
        
        __fs_makeSectorAction(i, 1, buffer, Write, drive);

        while(i < boot->tableSize) {
            __fs_makeSectorAction(i + boot->tableSector, 1, res.table + (i * 512), Write, drive);
            i++;
        }

        if(percentage) {
            *percentage = 100;
        }

        return res;
    } else {
        tunnelfs_t res;
        res.bootsector = NULL;
        res.drive = -1;
        res.table = NULL;
        return res;
    }
}
int __fs_tunnelFindFileMeta(char *name, char *extension, tunnelfs_t fsInstance) {
    assert(name); assert(extension); assert(fsInstance.bootsector); assert(fsInstance.table);

    int i = 0;
    while(i < 8192) {
        if((strncmp(fsInstance.table->files[i].filename, name, 16) == 0) && (strncmp(fsInstance.table->files[i].extension, extension, 4))) {
            return i;
        }
        i++;
    }
    return -1;
}
uint8_t *__fs_tunnelReadDataFromMeta(int id, tunnelfs_t fsInstance) {
    assert(fsInstance.bootsector); assert(fsInstance.table);

    tunnelfs_filemetadata_t meta = fsInstance.table->files[id];
    uint8_t *buffer = malloc(meta.sectorSize * 512);

    int i = 0;
    while(i < meta.sectorSize) {
        __fs_makeSectorAction(i + meta.sectorID, 1, buffer + (i * 512), Read, fsInstance.drive);
        i++;
    }

    return buffer;
}
int __fs_tunnelAllocateFile(char *name, char *extension, int sSize, tunnelfs_t fsInstance) {
    assert(name); assert(extension); assert(fsInstance.bootsector); assert(fsInstance.table);

    int i = 0;
    while(i < 8192) {
        if(fsInstance.table->files[i].sectorID == 0) {
            if(fsInstance.bootsector->lastFreeSectorSize >= sSize) {
                fsInstance.table->files[i].sectorSize = sSize;
                fsInstance.table->files[i].sectorID = fsInstance.bootsector->lastFreeSectorID;
                fsInstance.table->files[i].realSize = 0;
                fsInstance.bootsector->sectorsAvaliable -= sSize;
                fsInstance.bootsector->lastFreeSectorID += sSize + 1;
                fsInstance.bootsector->filesCreated++;
                fsInstance.bootsector->lastFreeSectorSize -= sSize;

                memcpy(fsInstance.table->files[i].filename, name, 16);
                memcpy(fsInstance.table->files[i].extension, extension, 16);

                __fs_makeSectorAction(0, 1, fsInstance.bootsector, Write, fsInstance.drive);
                int i2 = 0;
                while(i2 < fsInstance.bootsector->tableSize) {
                    __fs_makeSectorAction(i2 + fsInstance.bootsector->tableSector, 1, fsInstance.table + (i2 * 512), Write, fsInstance.drive);
                    i2++;
                }

                return i;
            } else {
                return -1;
            }
        }
        i++;
    }

    return -1;
}
void __fs_tunnelFreeFile(int fID, tunnelfs_t fsInstance) {
    assert(fsInstance.bootsector); assert(fsInstance.table);

    tunnelfs_filemetadata_t *meta = &fsInstance.table->files[fID];

    fsInstance.bootsector->filesCreated--;
    fsInstance.bootsector->lastFreeSectorID -= meta->sectorSize + 1;
    fsInstance.bootsector->sectorsAvaliable += meta->sectorSize;
    fsInstance.bootsector->lastFreeSectorSize += meta->sectorSize;
    meta->sectorID = 0;
    meta->sectorSize = 0;
    meta->realSize = 0;
    
    __fs_makeSectorAction(0, 1, fsInstance.bootsector, Write, fsInstance.drive);
    int i2 = 0;
    while(i2 < fsInstance.bootsector->tableSize) {
        __fs_makeSectorAction(i2 + fsInstance.bootsector->tableSector, 1, fsInstance.table + (i2 * 512), Write, fsInstance.drive);
        i2++;
    }

    return;
}
void __fs_tunnelSaveFile(int fID, uint8_t *buffer, tunnelfs_t fsInstance) { 
    tunnelfs_filemetadata_t *meta = &fsInstance.table->files[fID];
    
    int i2 = 0;
    while(i2 < meta->sectorSize) {
        __fs_makeSectorAction(i2 + meta->sectorID, 1, buffer + (i2 * 512), Write, fsInstance.drive);
        i2++;
    }

    return;
}