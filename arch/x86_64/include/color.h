#pragma once

#include "stdio.h"

typedef struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

// By SaziX
#define COLOR_RED           (color_t){255, 0, 0}
#define COLOR_GREEN         (color_t){0, 255, 0}
#define COLOR_BLUE          (color_t){0, 0, 255}
#define COLOR_WHITE         (color_t){255, 255, 255}
#define COLOR_BLACK         (color_t){0, 0, 0}
#define COLOR_DARK_GRAY     (color_t){128, 128, 128}
#define COLOR_DARK_GREEN    (color_t){0, 128, 0}
#define COLOR_GRAY          (color_t){192, 192, 192}
#define COLOR_LIGHT_GREEN   (color_t){70, 194, 29}
#define COLOR_YELLOW        (color_t){255, 242, 0}
#define COLOR_LIGHT_GRAY    (color_t){189, 189, 185}