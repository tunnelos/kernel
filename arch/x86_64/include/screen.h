#pragma once

#include "./stdint.h"
#include "./main.h"
#include "./tags.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#ifdef __cplusplus
extern "C" {
#endif

extern volatile unsigned char _binary_gui_psf_start;

typedef struct __attribute__((packed)) psf2_t {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;

extern uint8_t fb;

#ifdef __cplusplus
}
#endif