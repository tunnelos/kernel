#include "../include/tunnel.h"
#include "../include/video.h"
#include "../include/panic.h"

uint32_t bars[5] = {
    0x1F0, 0x3F6, 0x170, 0x376, 0x000
};

void __esi_setup_basic_hardware() {
    if(!__cpuid_check_sse()) {
        __serial_write_fmt("CPU %d -> tos > SSE is unavaliable!\r\n", __tools_get_cpu() - 1);
        while(1);
    }

    __cli();
    __idt_init();
    __sti();
    __sse_init();
    //__pic_unmask(0);
    __ide_init(bars);

    if(__cpuid_check_avx() || __cpuid_check_avx2()) {
        tunnelos_sysinfo.avx = true;
        __avx_init();
    } else {
        tunnelos_sysinfo.avx = false;
        __serial_write_fmt("CPU %d -> tos > AVX is unavaliable! (%d %d)\r\n", __tools_get_cpu() - 1, __cpuid_check_avx(), __cpuid_check_avx2());
    }

    tunnelos_sysinfo.interrupts = true;
    tunnelos_sysinfo.pit = true;
    tunnelos_sysinfo.rtc = true;
    tunnelos_sysinfo.nmi = false;
    tunnelos_sysinfo.ide = true;

    __video_setup_core();

    //crash("lmao", 6969, false);
}