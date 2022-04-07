#include "./include/idt.h"
#include "./include/tools.h"
#include "./include/smt.h"

idt_entry_t __idt_idt[256];
idtr_t __idt_idtr;
bool vectors[32];


void __idt_exception_handler() {
    //__serial_write_fmt("CPU %d -> tos > Exception!\r\n", __tools_get_cpu() - 1);
    if(__tools_get_cpu() > 1) {
        cores[__tools_get_cpu() - 1].exception = true;
        cores[__tools_get_cpu() - 1].busy = false;
    }
    __asm__ volatile("iretq");
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