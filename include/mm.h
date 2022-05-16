#pragma once

#include "./tunnel.h"
#include "./main.h"

extern int __mm_pointer;
extern int __mm_index;

void __mm_fillblocks();
void *malloc(int size);
bool free(void *address);
//does not include chain data
tunnel_memory_block_t __mm_get_blockinformation(void *address);
void *realloc(void *address, int size);