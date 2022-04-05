#pragma once

#include "./stdint.h"
#include "./serial.h"

typedef struct {
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t	ist;
	uint8_t attributes;
	uint16_t isr_mid;
	uint32_t isr_high;
	uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10))) extern idt_entry_t __idt_idt[256];
extern idtr_t __idt_idtr;
extern void *__idt_stub_table[32];
extern bool vectors[32];

void __idt_exception_handler();
void __idt_interrupt_handler();

void __idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
void __idt_init();