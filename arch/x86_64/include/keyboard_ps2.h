#pragma once

#include "./stdint.h"
#include "./arch.h"

#if GCCARCH_NUM == 0
#define KPS2_OB 0b00000001
#define KPS2_IB 0b00000010
#define KPS2_SF 0b00000100
#define KPS2_CD 0b00001000
#define KPS2_UL 0b00010000
#define KPS2_UT 0b00100000
#define KPS2_TE 0b01000000
#define KPS2_PE 0b10000000
#define KPS2_DP 0x60
#define KPS2_SR 0x64
#endif

extern char __keyboard_ps2_lookuptable[256];
extern bool __keyboard_ps2_ascii_only[256];
extern char __keyboard_ps2_buffer[256];
extern char *sc_name[256];
extern uint16_t __keyboard_ps2_buffer_pointer;

bool __keyboard_ps2_init();
void __keyboard_ps2_append_cin(char c);