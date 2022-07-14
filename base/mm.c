#include "../include/mm.h"
#include "../include/serial.h"
#include "../include/tools.h"
#include "../include/math.h"
#include "../include/stdint.h"
#include "../include/stdlib.h"

int __mm_pointer = 0;
int __mm_index = 0;

void *memcpy(void *dest, const void *src, size_t n) {
    size_t i = 0;
    char *d = (char *)dest;
    char *s = (char *)src;
    while(i < n) {
        d[i] = s[i];
        i++;
    }
    return dest;
}
void memset(void *dest, int c, size_t n) {
    size_t i = 0;
    char *d = (char *)dest;
    while(i < n) {
        d[i] = c % 256;
        i++;
    }
    return;
}

void __mm_fillblocks() {
    int m = 4096 * 8;
    int i = 0;
    while(i < m) {
        tunnelos_sysinfo.mm->meta[i].next = (struct tunnel_memory_block_t *)(&tunnelos_sysinfo.mm->meta[i + 1]);
        tunnelos_sysinfo.mm->meta[i].prev = (struct tunnel_memory_block_t *)(&tunnelos_sysinfo.mm->meta[i - 1]);
        tunnelos_sysinfo.mm->meta[i].address = &tunnelos_sysinfo.mm->blockdata[i];
        tunnelos_sysinfo.mm->meta[i].id = -1;
        tunnelos_sysinfo.mm->meta[i].have = 0;
        tunnelos_sysinfo.mm->meta[i].free = true;
        i++;
    }
    return;
}

void *malloc(int size) {
    #if TUNNEL_TRANDOM <= 0
    srand(1024);
    #endif
    if(size <= 0) return 0;
    int state[6];
    state[4] = round(size / 256);
    if(state[4] == 0) state[4] = 1;
    //state[4] - blocks to allocate
    //state[0] - block offset
    //__mm_index - block offset
    state[3] = 0;
    state[2] = 0;
    state[1] = 0;
    state[0] = __mm_index;
    __mm_index += state[4];

    while(state[3] < 4096 * 8) {
        if(!tunnelos_sysinfo.mm->meta[state[3]].free) state[2] = 0;
        else state[2]++;        
        state[3]++;
        if(state[2] == state[4]) {
            tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].have = state[2];
            while(state[1] < state[2]) {
                tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].free = false;
                #if TUNNEL_TRANDOM >= 0
                tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].id = (uint16_t)TUNNEL_RANDOM();
                if(tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].id <= 0) tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].id = tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].id + tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].id + tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].id;
                #else
                tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].id = rand() % 4096;
                #endif
                tunnelos_sysinfo.mm->meta[state[1] + state[3] + state[0]].have = state[2] - state[1] - 1;
                state[6] = state[1] + state[3] + state[0];
                state[1]++;
            }
            __serial_write_fmt("CPU %d -> tos > Allocated %d blocks of memory on address %X as %d\r\n", __tools_get_cpu() - 1, state[2], (uint64_t)tunnelos_sysinfo.mm->meta[state[6]].address, tunnelos_sysinfo.mm->meta[state[6]].id);
            return tunnelos_sysinfo.mm->meta[state[6]].address;
        }
        state[3]++;
    }
    return 0;
}

tunnel_memory_block_t __mm_get_blockinformation(void *address) {
    uint64_t adr = (uint64_t)address;
    uint64_t sadr = (uint64_t)(&tunnelos_sysinfo.mm->blockdata);
    uint64_t index = adr - sadr;
    uint64_t blk = floor(index / 256);
    tunnel_memory_block_t bl;
    bl.address = tunnelos_sysinfo.mm->meta[blk].address;
    bl.free = tunnelos_sysinfo.mm->meta[blk].free;
    bl.have = tunnelos_sysinfo.mm->meta[blk].have;
    return bl;
}

void *realloc(void *address, int size) {
    void *new_addr = malloc(size);
    void *old = address;
    int old_size = __mm_get_blockinformation(address).have;
    if(old_size == 0) old_size = 1;
    old_size *= 256;
    uint64_t i = 0;
    char *data_new = (char *)new_addr;
    char *data_old = (char *)old;
    while(i < old_size) {
        data_new[i] = data_old[i];
        i++;
    }
    free(address);
    __serial_write_fmt("CPU %d -> tos > Reallocated %d blocks\r\n", __tools_get_cpu() - 1, old_size / 256);
    return new_addr;
}

bool free(void *address) {
    uint64_t adr = (uint64_t)address;
    uint64_t sadr = (uint64_t)(&tunnelos_sysinfo.mm->blockdata);
    uint64_t index = adr - sadr;
    uint64_t blk = floor(index / 256);
    int blks = 0;
    if(blk >= 4096 * 8) return false;
    if(blk < 0) return false;
    if(tunnelos_sysinfo.mm->meta[blk].free) return false;
    int i = 0;
    blks = tunnelos_sysinfo.mm->meta[blk].have;
    tunnelos_sysinfo.mm->meta[blk].id = -1;
    if(blks == 0) blks = 1;
    while(i < blks) {
        tunnelos_sysinfo.mm->meta[blk + i].free = true;
        tunnelos_sysinfo.mm->meta[blk + i].id = -1;
        i++;
    }
    __serial_write_fmt("CPU %d -> tos > Freed %d blocks\r\n", __tools_get_cpu() - 1, blks);
    return true;
}
