#include "../include/pic.h"
#include "../include/tools.h"

void __pic_eoi(uint8_t irq) {
    if(irq >= 8) outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
    return;
}
void __pic_remap(int *offset) {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset[0]);
	io_wait();
	outb(PIC2_DATA, offset[1]);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
    
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}
void __pic_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if(irq < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}
void __pic_unmask(uint8_t irq) {
    uint16_t value;
    uint8_t port;

    if(irq < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}
uint16_t __pic_get_irq_reg(int ocw3) {
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    io_wait();
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}
uint16_t __pic_get_irr() {
    return __pic_get_irq_reg(PIC_READ_IRR);
}
uint16_t __pic_get_isr() {
    return __pic_get_irq_reg(PIC_READ_ISR);
}