#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define TUNNELFS_1 0x10

#pragma pack(push, 1)

typedef enum {
    GCCMin00 = 0x00,

    SingleSided8           = 0xE5,
    DoubleSided5           = 0xED,
    DRDos_CustomPartitions = 0xEE,
    SuperFloppy            = 0xEF,
    DoubleSided3           = 0xF0,
    Altos_DoubleDensity    = 0xF4,
    Altos_FixedDisk        = 0xF5,
    HardDrivePartition     = 0xF8,
    DoubleSided3_9Sectors  = 0xF9,
    DoubleSided3_8Sectors  = 0xFA,
    DoubleSided3_8Sectors1 = 0xFB,
    DoubleSided5_640KB     = 0xFC,
    DoubleSided5_360KB     = 0xFD,
    SingleSided5_160KB     = 0xFE,
    DoubleSided5_320KB     = 0xFF,
} __attribute__ ((packed)) MediaDescriptor;

typedef enum {
    GCCMin01 = 0x00,

    Signature1 = 0x28,
    Signature2 = 0x29,

    GCCMax01 = 0xFF
} __attribute__ ((packed)) FATSignature;

typedef enum {
    GCCMin02 = 0x00,

    ReadOnly = 0x01,
    Hidden = 0x02,
    System = 0x04,
    VolumeID = 0x08,
    Directory = 0x10,
    Archive = 0x20,
    LongFileName = 0x0F,

    GCCMax02 = 0xFF
} __attribute__ ((packed)) FATAttribute;

enum FATType {
    FAT12 = 0,
    FAT16,
    FAT32
};

typedef struct bpb_t {
    uint8_t onLoadCode[3];
    char oem[8];
    // Bytes Per Sector
    uint16_t bps;
    // Sectors Per Cluster
    uint8_t spc;
    uint16_t reservedSectors;
    uint8_t fat;
    uint16_t rootDirEntries;
    // FAT16
    uint16_t sectors;
    MediaDescriptor descriptor;
    uint16_t fat16Table;
    // Sectors Per Track
    uint16_t spt;
    uint16_t heads;
    uint32_t hiddenSectors;
    // FAT32
    uint32_t totalSectors;
} bpb_t;

typedef struct fat12_t {
    bpb_t bpb;
    uint8_t drive;
    uint8_t winNTFlags;
    FATSignature signature;
    uint32_t volumeID;
    char volumeLabel[11];
    char systemIDString[8];
    uint8_t bootCode[448];
    uint16_t bootablePartitionSignature;
} fat12_t;
typedef fat12_t fat16_t;

typedef struct fat32_t {
    bpb_t bpb;
    // FAT32
    uint16_t tableSize;
    uint16_t flags;
    uint16_t fatVersion;
    uint32_t rootCluster;
    uint16_t fsinfoSector;
    uint16_t backupBootSector;
    uint8_t reserved[12];
    uint8_t drive;
    uint8_t winNTFlags;
    FATSignature signature;
    uint32_t volumeID;
    char volumeLabel[11];
    char systemIDString[8];
    uint8_t bootCode[420];
    uint16_t bootablePartitionSignature;
} fat32_t;
typedef struct fat32_fsinfo_t {
    uint32_t signature1;
    uint8_t reserved1[480];
    uint32_t signature2;
    uint32_t lastFreeCluster;
    uint32_t search_ClusterNum;
    uint8_t reserved2[12];
    uint32_t signature3;
} fat32_fsinfo_t;

typedef struct fat_filename {
    char name[8];
    char ext[4];
} fat_filename;
typedef struct fat_directory {
    fat_filename filename;
    FATAttribute attributes;
    uint8_t winNTReserved;
    uint8_t creationTime_tenths;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t lastAccessedDate;
    uint16_t entry_firstClusterNum1;
    uint16_t lastModificationTime;
    uint16_t lastModificationDate;
    // Use this number to find the first cluster for this entry. 
    uint16_t entry_firstClusterNum2;
    uint32_t size;
} fat_directory;
typedef struct fat_lfn {
    uint8_t order;
    char entryCharacters0[10];
    FATAttribute attribute;
    uint8_t longEntryType;
    uint8_t checksum;
    char entryCharacters1[12];
    uint16_t zero;
    char entryCharacters2[4];
} fat_lfn;

typedef struct tunnelfs_bootsector_t {
    char signature[8];

    int sectorsAvaliable;
    int totalSectors;
    int lastFreeSectorID;
    int lastFreeSectorSize;

    int tableSector;
    int tableSize; //in sectors

    int filesCreated;

    uint8_t revision;

    char freeSpace[475];
} tunnelfs_bootsector_t;

typedef struct tunnelfs_filemetadata_t {
    char filename[16];
    char extension[4];
    int sectorSize;
    int realSize;
    int sectorID;
} tunnelfs_filemetadata_t;
typedef struct tunnelfs_table_t {
    tunnelfs_filemetadata_t files[8192];
} tunnelfs_table_t;

typedef struct tunnelfs_t {
    tunnelfs_bootsector_t *bootsector;
    tunnelfs_table_t *table;
    uint8_t drive;
} tunnelfs_t;

#pragma pack(pop)

enum SectorAction {
    Read = 0,
    Write
};

void *__fs_makeSectorAction(int sID, int sSize, void *buffer, enum SectorAction action, uint8_t drive);

bool __fs_tunnelFindFS(uint8_t drive);
tunnelfs_t __fs_tunnelCreateFS(int *percentage, uint8_t drive);
int __fs_tunnelFindFileMeta(char *name, char *extension, tunnelfs_t fsInstance);
uint8_t *__fs_tunnelReadDataFromMeta(int id, tunnelfs_t fsInstance);
int __fs_tunnelAllocateFile(char *name, char *extension, int sSize, tunnelfs_t fsInstance);
void __fs_tunnelFreeFile(int fID, tunnelfs_t fsInstance);
void __fs_tunnelSaveFile(int fID, uint8_t *buffer, tunnelfs_t fsInstance);

// Returns if read was successful or not
bool __fs_readFATCheck(uint8_t drive);
enum FATType __fs_getFATType(uint8_t drive);

#ifdef __cplusplus
}
#endif