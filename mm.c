#include "./include/mm.h"
#include "./include/serial.h"
#include "./include/tools.h"

int __mm_pointer = 0;



tunnel_memory_block_t malloc(int size) {
    tunnel_memory_block_t r;
    r.size = size;
    r.buffer_index = __mm_pointer;
    r.address = &tunnelos_sysinfo.mm->alloc_table[__mm_pointer];
    __serial_write_fmt("CPU %d -> tos > %d bytes have allocated on address 0x%X\r\n", __tools_get_cpu() - 1, size, r.address);
    __mm_pointer += size;
    return r;
}

void malloc_new(int size) {
    return;
}