#pragma once

#define PORT 0x3F8
#include "./stdint.h"
#include "./tools.h"
#include "./cstring.h"
#include "./stdio.h"
#include <stdarg.h>

bool __serial_init();
int __serial_got_signal();
char __serial_read();
int __serial_transmit_check();
void __serial_write_char(char a);
void __serial_write_raw(const char *data, uint64_t size);
void __serial_write_fmt(const char *fmt, ...);