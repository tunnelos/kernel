#include "stdio.h"

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

// By SaziX
#define COLOR_RED           (Color){255, 0, 0}
#define COLOR_GREEN         (Color){0, 255, 0}
#define COLOR_BLUE          (Color){0, 0, 255}
#define COLOR_WHITE         (Color){255, 255, 255}
#define COLOR_BLACK         (Color){0, 0, 0}
#define COLOR_DARK_GRAY     (Color){128, 128, 128}
#define COLOR_DARK_GREEN    (Color){0, 128, 0}
#define COLOR_GRAY          (Color){192, 192, 192}
#define COLOR_LIGHT_GREEN   (Color){70, 194, 29}
#define COLOR_YELLOW        (Color){255, 242, 0}
#define COLOR_LIGHT_GRAY    (Color){189, 189, 185}

uint32_t __color_to_int(Color color) {
    return (color.r * 256 * 256) + (color.g * 256) + color.b;
}