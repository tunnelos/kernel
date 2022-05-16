#include "./include/mm.h"
#include "./include/serial.h"
#include "./include/tools.h"
#include "./include/math.h"

int __mm_pointer = 0;
int __mm_index = 0;

void __mm_fillblocks() {
    int m = 4096 * 8;
    int i = 0;
    while(i < m) {
        tunnelos_sysinfo.mm->meta[i].next = (struct tunnel_memory_block_t *)(&tunnelos_sysinfo.mm->meta[i + 1]);
        tunnelos_sysinfo.mm->meta[i].prev = (struct tunnel_memory_block_t *)(&tunnelos_sysinfo.mm->meta[i - 1]);
        tunnelos_sysinfo.mm->meta[i].address = &tunnelos_sysinfo.mm->blockdata[i];
        tunnelos_sysinfo.mm->meta[i].free = true;
        i++;
    }
    return;
}

void *malloc(int size) {
    if(size <= 0) return 0;
    int state[5];
    state[4] = round(size / 256);
    if(state[4] == 0) state[4] = 1;
    state[3] = 0;
    state[2] = 0;
    state[1] = 0;

    while(state[3] < 4096 * 8) {
        if(!tunnelos_sysinfo.mm->meta[state[3]].free) state[2] = 0;
        else state[2]++;
        state[3]++;
        if(state[2] == state[4]) {
            tunnelos_sysinfo.mm->meta[state[1] + state[3]].have = state[2];
            while(state[1] < state[2]) {
                tunnelos_sysinfo.mm->meta[state[1] + state[3]].free = false;
                tunnelos_sysinfo.mm->meta[state[1] + state[3]].have = state[2] - state[1] - 1;
                state[0] = state[1] + state[3];
                state[1]++;
            }
            __serial_write_fmt("Allocated %d blocks of memory on address %X\r\n", state[2], (uint64_t)tunnelos_sysinfo.mm->meta[state[0]].address);
            return tunnelos_sysinfo.mm->meta[state[0]].address;
        }
        state[3]++;
    }
    return 0;
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
    if(blks == 0) blks = 1;
    while(i < blks) {
        tunnelos_sysinfo.mm->meta[blk + i].free = true;
        i++;
    }
    __serial_write_fmt("Freed %d blocks\r\t", blks);
    return true;
}