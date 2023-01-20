#pragma once

#include "./stdint.h"
#include "./serial.h"
#include "./registers.h"

#define IDT_INTERRUPT_PIT  0
#define IDT_INTERRUPT_CMOS 8

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct idt_entry_t {
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t	ist;
	uint8_t attributes;
	uint16_t isr_mid;
	uint32_t isr_high;
	uint32_t reserved;
} idt_entry_t;

typedef struct interrupt_frame_t {
	uint64_t rip;
	uint64_t cs;
	uint64_t flags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t err;
	uint64_t interrupt_id;
} interrupt_frame_t;

typedef struct idtr_t {
	uint16_t limit;
	uint64_t base;
} idtr_t;

typedef struct interrupt_t {
	int interrupt_id;
	interrupt_frame_t *frame;
	bool critical;
	bool active;
} interrupt_t;
#pragma pack(pop)

__attribute__((aligned(0x10))) extern idt_entry_t __idt_idt[256];
extern idtr_t __idt_idtr;
extern void *__idt_stub_table[32];
extern bool vectors[32];
extern interrupt_t current_interrupt;
extern interrupt_frame_t *current_iframe;

void __idt_exception_handler(int interrupt_id, register_set_t *register_set);
void __idt_interrupt_handler(int interrupt_id, register_set_t *register_set);

void __idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
void __idt_init();

extern void __cli();
extern void __sti();
extern void __hlt();
extern int  __read_error_code();

#ifdef __cplusplus
}
#endif