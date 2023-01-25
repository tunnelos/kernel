#include "../include/idt.h"
#include "../include/tools.h"
#include "../include/rtc.h"
#include "../include/cmos.h"
#include "../include/panic.h"
#include "../include/tunnel.h"
#include "../include/nmi.h"
#include "../include/tunnel.h"
#include "../include/pit.h"

idt_entry_t __idt_idt[256];
idtr_t __idt_idtr;
bool vectors[32];
interrupt_t current_interrupt;
interrupt_frame_t *current_iframe;

void __idt_exception_handler(int interrupt_id, register_set_t *register_set) {
    uint32_t errorCode = ((uint32_t *)__get_stack())[0];
    current_interrupt.active = true;
    current_interrupt.interrupt_id = interrupt_id;
    current_interrupt.critical = true;
    current_interrupt.frame = current_iframe;
    if(interrupt_id != 0) __serial_write_fmt("CPU %d -> tos > Exception %d!\r\n * RIP = %l %X\r\n * Error Code = %d\r\n", __tools_get_cpu() - 1, interrupt_id, current_iframe->rip, errorCode);
    switch(interrupt_id) {
        case 8: return;
        case 18: return;
        default: {
            crash(PANIC_UNEXPECTED_INTERRUPT_STRING, PANIC_UNEXPECTED_INTERRUPT_NUMBER, true);
            break;
        }
    }
    current_interrupt.active = false;
}
uint64_t tt = 0;
int ti = 0;
void __idt_interrupt_handler(int interrupt_id, register_set_t *register_set) {
    current_interrupt.active = true;
    current_interrupt.critical = false;
    current_interrupt.interrupt_id = interrupt_id;
    current_interrupt.frame = current_iframe;

    // task switching should be done quickly

    if(interrupt_id == IDT_INTERRUPT_PIT) {
        __pit_event_timer();
        __pit_eoi();
        // if((tt % 1000000) == 0) {
            __serial_write_fmt("CPU %d -> tos > Interrupt %d! %d\r\n", __tools_get_cpu() - 1, interrupt_id, tt);
            __serial_write_fmt("- CPU %d -> tos > PIT fractions: %d\r\n", __tools_get_cpu() - 1, __pit_IRQ0_fractions);
            __serial_write_fmt("- CPU %d -> tos > PIT ms: %d\r\n", __tools_get_cpu() - 1, __pit_IRQ0_ms);
            __serial_write_fmt("- CPU %d -> tos > PIT frequency: %d\r\n", __tools_get_cpu() - 1, __pit_IRQ0_frequency);
            __serial_write_fmt("- CPU %d -> tos > PIT rvalue: %d\r\n", __tools_get_cpu() - 1, __pit_reload_value);
            ti++;
        // }
        tt++;
    } else {
        __serial_write_fmt("CPU %d -> tos > Interrupt %d!\r\n * RIP = %l %X\r\n", __tools_get_cpu() - 1, interrupt_id, register_set->rip);
        switch(interrupt_id) {
            case IDT_INTERRUPT_PIT: {
                __pit_event_timer();
                __pit_eoi();
                break;
            }
            case IDT_INTERRUPT_CMOS: {
                if(!tunnelos_sysinfo.rtc) {
                    __nmi_init();
                    __rtc_init();
                    tunnelos_sysinfo.rtc = true;
                }
                if(__cmos_firstInt) {
                    __cmos_getRTC();
                    __cmos_firstInt = false;
                }
                outb(RTC_REGISTER_B_OUT, 0x0C);
                inb(RTC_REGISTER_B_IN);
                break;
            }
            case 6: {
                break;
            }
            default: {
                crash(PANIC_UNEXPECTED_INTERRUPT_STRING, PANIC_UNEXPECTED_INTERRUPT_NUMBER, true);
                break;
            }
        }   
    }
    current_interrupt.active = false;
}

void __idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags) {
    idt_entry_t *d = &__idt_idt[vector];

    d->attributes = flags;
    d->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    d->isr_low = (uint64_t)isr & 0xFFFF;
    d->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    d->ist = 0;
    d->kernel_cs = 8;
    d->reserved = 0;
}

void __idt_init() {
    __idt_idtr.base = (uint64_t)&__idt_idt[0];
    __idt_idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;

    for(uint8_t vector = 0; vector < 32; vector++){
        __idt_set_descriptor(vector, __idt_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile("lidt %0" : : "m"(__idt_idtr));
    __asm__ volatile("sti");

    __serial_write_fmt("CPU %d -> tos > Interrupts are enabled\r\n", __tools_get_cpu() - 1);
}