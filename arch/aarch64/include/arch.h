#pragma once

#include "flags.h"

#if defined(__x86_64__)
#define GCCARCH_NUM 0
#define GCCARCH_STR "x86_64"
#elif defined(__aarch64__)
#define GCCARCH_NUM 1
#define GCCARCH_STR "aarch64"
#else
#define GCCARCH_NUM 2
#define GCCARCH_STR "unknown"
#error "Unsupported architecture"
#endif
#if defined(__APPLE__) 
#define GCCENVIRONMENT_STR "Mac OS"
#define GCCENVIRONMENT_NUM 0
#elif defined(__linux__)
#define GCCENVIRONMENT "Linux"
#define GCCENVIRONMENT_NUM 1
#else
#define GCCENVIRONMENT "Unknown"
#define GCCENVIRONMENT_NUM 2
#endif

extern const char *__arch_archlookup[3];
extern const char *__arch_envlookup [3];

#if OPTIMIZE == 0
const char *__arch_getarch();
const char *__arch_getenvironment();
#endif