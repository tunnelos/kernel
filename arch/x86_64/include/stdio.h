#pragma once

#include "./main.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void FILE;

int sprintf(char *str, const char *fmt, ...);

// ASCII font with GUI additions in transparent mode
void puts(const char *str, uint32_t color, int x, int y);
// ASCII font with GUI additions in transparent mode
void putc(const char c, uint32_t color, int x, int y);

void printf(uint32_t color, int x, int y, const char *fmt, ...);

extern int __stdio_margin;
extern int __stdio_gui_margin;
extern bool terminal_block;

char *itoa(int num, char *buffer, int base, int x, int y, int color, bool use_additional);
char *itoalong(uint64_t num, char *buffer, int base, int x, int y, int color, bool use_additional);
char itoh(int num, bool upper, int x, int y, int color, bool use_additional);

char *stditoa(int num, char *buffer, int base);
char *stditoalong(uint64_t num, char *buffer, int base);
char stditoh(int num, bool upper);

char itoc(int num);
char* strrev(char* src);

FILE *fopen(const char *path, const char *mode); // Stub
int fscanf(FILE *stream, const char *format, ...); // Stub
int feof(FILE *stream); // Stub
int fgetc(FILE *stream); // Stub

void perror(const char *s);

#ifdef __cplusplus
}
#endif