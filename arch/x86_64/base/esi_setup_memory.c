#include "../include/tunnel.h"
#include "../include/bootboot.h"
#include "../include/tools.h"
#include "../include/serial.h"
#include "../include/mm.h"

void __esi_setup_memory() {
    MMapEnt *mmap_ent = &bootboot.mmap;
    while(mmap_ent < (MMapEnt *)(&bootboot) + (&bootboot)->size) {
        __serial_write_fmt("CPU %d -> tos > Check memory type: %d\r\n", __tools_get_cpu() - 1, MMapEnt_Type(mmap_ent));
        if(MMapEnt_Type(mmap_ent) == MMAP_FREE) {
            tunnelos_sysinfo.free_memory_location_size = mmap_ent->size;
            __serial_write_fmt("CPU %d -> tos > Free memory size: %d KB\r\n", __tools_get_cpu() - 1, tunnelos_sysinfo.free_memory_location_size / 1024);
            if(tunnelos_sysinfo.free_memory_location_size / 1024 > 19000) {
                tunnelos_sysinfo.mm = (tunnel_memory_map_t *)((void *)(mmap_ent->ptr));
                __serial_write_fmt("CPU %d -> tos > Found one at address %l0x%X\r\n", __tools_get_cpu() - 1, mmap_ent->ptr);
                break;
            }
            //break;
        }

        mmap_ent++;
    }

    __mm_fillblocks();
    
    // tunnelos_sysinfo.mm = (tunnel_memory_map_t *)((void *)0x131000);

    // tunnelos_sysinfo.mm = (tunnel_memory_map_t *)((MMapEnt *)(&bootboot.mmap + 4)->ptr);
    // tunnelos_sysinfo.mm->start_point = ((MMapEnt *)(&bootboot.mmap + 4))->size;
}