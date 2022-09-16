#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define __SIZE_TYPE__ long unsigned int

typedef unsigned char uint8_t;
#ifndef __cplusplus
typedef uint8_t bool;
#endif
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;
typedef __int128_t int128_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef __uint128_t uint128_t;
typedef __SIZE_TYPE__ size_t;

#define true (bool)1
#define false (bool)0
#define NAN 0.0/0.0
#define NaN NAN
#define NULL (void *)0

#define ULONG_MAX 0xFFFFFFFFUL
#define INT_MAX   2147483647
#define INT_MIN  -2147483647

#pragma pack(push, 1)
typedef struct BMPHeader {    // Total: 54 bytes
  uint16_t  type;             // Magic identifier: 0x4d42
  uint32_t  size;             // File size in bytes
  uint16_t  reserved1;        // Not used
  uint16_t  reserved2;        // Not used
  uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
  uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
  int32_t   width_px;         // Width of the image
  int32_t   height_px;        // Height of image
  uint16_t  num_planes;       // Number of color planes
  uint16_t  bits_per_pixel;   // Bits per pixel
  uint32_t  compression;      // Compression type
  uint32_t  image_size_bytes; // Image size in bytes
  int32_t   x_resolution_ppm; // Pixels per meter
  int32_t   y_resolution_ppm; // Pixels per meter
  uint32_t  num_colors;       // Number of colors  
  uint32_t  important_colors; // Important colors 
} BMPHeader;

typedef struct BMPImage {
  BMPHeader header;
  unsigned char* data; 
} BMPImage;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct cmos_t {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
    uint32_t century;
} cmos_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct vector2d_t {
    int x;
    int y;
} vector2d_t;

enum DrawTask {
    Rectangle = 0,
    Text, Pixel
};
enum DrawState {
    Idle = 0,
    Unused, Active,
};
typedef struct drawtask_t {
    vector2d_t pos;
    vector2d_t size;
    
    char *text;
    int color;

    enum DrawTask task;
    enum DrawState state;
} drawtask_t;

enum CoreshellInstallationState {
    NotInstalled = 0,
    NotConfigured, Ready
};
typedef struct coreshell_uperms_t {
    bool loginWithoutPassword;
    bool useAccount;
} coreshell_uperms_t;
typedef struct coreshell_user_t {
    char name[16];
    char password[16];
    int uid;
    coreshell_uperms_t permissions;
    bool avaliable;
    char reserved[47];
} coreshell_user_t;
typedef struct coreshell_hddsettings_t {
    char signature[16];
    enum CoreshellInstallationState installstate;
    coreshell_user_t users[4];
    uint8_t rev;
    uint16_t turnedOnTimes;
    bool recoverFromError;
    char freeSpace[144];
} coreshell_hddsettings_t;
#pragma pack(pop)

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

typedef struct unitype_t unitype_t;
struct unitype_t {
	void *p;
	unitype_t **next;
	unitype_t **prev;
	const char *name;
	char type;
	int elements;
};
typedef struct {
	unitype_t *(* func)(unitype_t *args);
	const char *name;
} unifunc_t;

#pragma pack(push, 1)
typedef struct halmember_t {
    // Index at 0 for "functions" is init function
    unitype_t *functions;
    unitype_t *variables;
    const char *name;
} halmember_t;

typedef struct halcategory_t {
    halmember_t *members;
    const char *name;
} halcategory_t;

typedef struct halstructure_t {
    halcategory_t *categories;
} halstructure_t;
#pragma pack(pop)

enum SectorAction {
    Read = 0,
    Write = 1
};

typedef struct ide_channel_reg_t {
    uint16_t base;
    uint16_t ctrl;
    uint16_t bmide;
    uint8_t no_int;
} ide_channel_reg_t;
typedef struct ide_dev_t {
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
typedef struct ide_rw_t {
    //0 - read | 1 - write
    bool rw;
    uint8_t drive;
    uint32_t lba;
    uint8_t sectors;
    uint16_t selector;
    uint32_t buffer;
} ide_rw_t;

#pragma pack(push, 1)
typedef struct idt_entry_t {
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t	ist;
	uint8_t attributes;
	uint16_t isr_mid;
	uint32_t isr_high;
	uint32_t reserved;
} idt_entry_t;

typedef struct interrupt_frame_t {
	uint64_t rip;
	uint64_t cs;
	uint64_t flags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t err;
	uint64_t interrupt_id;
} interrupt_frame_t;

typedef struct idtr_t {
	uint16_t limit;
	uint64_t base;
} idtr_t;

typedef struct interrupt_t {
	int interrupt_id;
	interrupt_frame_t *frame;
	bool critical;
	bool active;
} interrupt_t;
#pragma pack(pop)

typedef struct pit_timer_t {
    bool avaliable: true;
    uint64_t every;
    bool multiple;
    void (*callback)(void);
    uint64_t next_on;
} pit_timer_t;

typedef struct __attribute__((packed)) psf2_t {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;

typedef char* string;

#pragma pack(push, 1)
typedef struct {
  uint64_t   ptr;
  uint64_t   size;
} MMapEnt;

typedef struct {
  /* first 64 bytes is platform independent */
  uint8_t    magic[4];    /* 'BOOT' magic */
  uint32_t   size;        /* length of bootboot structure, minimum 128 */
  uint8_t    protocol;    /* 1, static addresses, see PROTOCOL_* and LOADER_* above */
  uint8_t    fb_type;     /* framebuffer type, see FB_* above */
  uint16_t   numcores;    /* number of processor cores */
  uint16_t   bspid;       /* Bootsrap processor ID (Local APIC Id on x86_64) */
  int16_t    timezone;    /* in minutes -1440..1440 */
  uint8_t    datetime[8]; /* in BCD yyyymmddhhiiss UTC (independent to timezone) */
  uint64_t   initrd_ptr;  /* ramdisk image position and size */
  uint64_t   initrd_size;
  uint64_t   fb_ptr;      /* framebuffer pointer and dimensions */
  uint32_t   fb_size;
  uint32_t   fb_width;
  uint32_t   fb_height;
  uint32_t   fb_scanline;

  /* the rest (64 bytes) is platform specific */
  union {
    struct {
      uint64_t acpi_ptr;
      uint64_t smbi_ptr;
      uint64_t efi_ptr;
      uint64_t mp_ptr;
      uint64_t unused0;
      uint64_t unused1;
      uint64_t unused2;
      uint64_t unused3;
    } x86_64;
    struct {
      uint64_t acpi_ptr;
      uint64_t mmio_ptr;
      uint64_t efi_ptr;
      uint64_t unused0;
      uint64_t unused1;
      uint64_t unused2;
      uint64_t unused3;
      uint64_t unused4;
    } aarch64;
  } arch;

  /* from 128th byte, MMapEnt[], more records may follow */
  MMapEnt    mmap;
  /* use like this:
   * MMapEnt *mmap_ent = &bootboot.mmap; mmap_ent++;
   * until you reach bootboot->size, while(mmap_ent < bootboot + bootboot->size) */
} BOOTBOOT;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct tunnel_memory_block_t {
    bool free;
    void *address;
    int have;
    int id;
} tunnel_memory_block_t;
typedef struct tunnel_memory_map_t {
    tunnel_memory_block_t meta[8320*8];
    char blockdata[8320*8][256];
    uint64_t start_point;
} tunnel_memory_map_t;
typedef struct tunnelos_sysinfo_t {
    BOOTBOOT bootboot;
    uint8_t *free_memory_location;
    uint64_t free_memory_location_size;
    tunnel_memory_map_t *mm;
    bool avx;
    bool sse;
    bool sse2;
    bool rtc;
    bool interrupts;
    bool nmi;
    bool pit;
    bool ide;
    int cores;
} tunnelos_sysinfo_t;
#pragma pack(pop)

typedef struct uhci_descriptor_t {
    uint32_t next;
    uint32_t status;
    uint32_t packet;
    uint32_t buffer;
    uint128_t system;
} uhci_descriptor_t;

typedef struct ui_t {
    /* data */
} ui_t;

typedef struct ui_dev_t {

} ui_dev_t;
typedef struct ui_theme_t {

} ui_theme_t;
typedef struct ui_col_t {
    int primary;
    int secondary;
    int tertiary;
} ui_col_t;

#ifdef __cplusplus
}
#endif