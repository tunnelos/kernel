#pragma once

#include "./main.h"

//standart text
void puts(char *str, uint32_t color, int x, int y);
void putc(char c, uint32_t color, int x, int y);

void puts_gui(char *str, uint32_t color, int x, int y);
void putc_gui(char c, uint32_t color, int x, int y);

void printf(uint32_t color, int x, int y, const char *fmt, ...);

extern int __stdio_margin;
extern int __stdio_gui_margin;
extern bool terminal_block;

char *itoa(int num, char *buffer, int base, int x, int y, int color, bool use_additional);
char itoh(int num, bool upper, int x, int y, int color, bool use_additional);
char itoc(int num);
char* strrev(char* src);