#pragma once

typedef unsigned char uint8_t;
typedef uint8_t bool;
typedef short int int16_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef __uint128_t uint128_t;
typedef uint32_t size_t;

#define true (bool)1
#define false (bool)0
#define NAN 0.0/0.0
#define NaN NAN
#define NULL (void *)0

#define ULONG_MAX 0xFFFFFFFFUL
#define INT_MAX   2147483647
#define INT_MIN  -2147483647