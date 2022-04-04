#include "./include/keyboard_ps2.h"
#include "./include/tools.h"

char __keyboard_ps2_lookuptable[256] = { '?', '?', '1', '2', '3', '4', '5', '6',
                                        '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
                                        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
                                        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                                        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' ' };
char __keyboard_ps2_buffer[256];
uint16_t __keyboard_ps2_buffer_pointer;

bool __keyboard_ps2_init() {
    return (inb(KPS2_SR) & KPS2_SF) ? true : false;
}
void __keyboard_ps2_append_cin(char c) {
    __keyboard_ps2_buffer[__keyboard_ps2_buffer_pointer++] = c;
    return;
}