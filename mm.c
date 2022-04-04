#include "./include/mm.h"

int __mm_pointer = 0;

tunnel_memory_block_t malloc(int size) {
    tunnel_memory_block_t r;
    r.size = size;
    r.address = &tunnelos_sysinfo.mm->alloc_table[__mm_pointer];
    __mm_pointer += size;
    return r;
}