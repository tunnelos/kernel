#pragma once

#include "./tunnel.h"
#include "./main.h"

extern int __mm_pointer;

tunnel_memory_block_t malloc(int size);