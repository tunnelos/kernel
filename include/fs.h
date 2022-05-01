#pragma once

#include "./stdint.h"

typedef struct {
  uint64_t signature;
  uint16_t block_allocation_table[259107][16];
  uint8_t files[512*1024*1024];
  uint64_t file_count;
} tfs_t;
typedef struct {
    char filename[256];
    uint16_t index;
    bool folder;
    uint16_t metadata;
} tfs_file_t;

extern tfs_t *__tunnelfs_instance;

void __tunnelfs_create();