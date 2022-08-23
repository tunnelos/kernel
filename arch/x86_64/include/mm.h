#pragma once

#include "./tunnel.h"
#include "./main.h"
#include "./tags.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __mm_pointer;
extern int __mm_index;

void *memcpy(void *dest, const void *src, size_t n);
void memset(void *dest, int c, size_t n);
void __mm_fillblocks();
int __mm_findoffset(int blocks);
void *malloc(size_t size);
void *calloc(size_t size);
void free(void *address);
tunnel_memory_block_t __mm_get_blockinformation(void *address);
void *realloc(void *address, size_t size);

#ifdef __cplusplus
}
#endif
