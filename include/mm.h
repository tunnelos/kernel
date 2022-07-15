#pragma once

#include "./tunnel.h"
#include "./main.h"

extern int __mm_pointer;
extern int __mm_index;

void *memcpy(void *dest, const void *src, size_t n);
void memset(void *dest, int c, size_t n);
void __mm_fillblocks();
int __mm_findoffset(int blocks);
void *malloc(int size);
bool free(void *address);
tunnel_memory_block_t __mm_get_blockinformation(void *address);
void *realloc(void *address, int size);
