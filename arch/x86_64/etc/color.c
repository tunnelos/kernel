#include "../include/color.h"

uint32_t __color_to_int(color_t color) {
    return (color.r * 256 * 256) + (color.g * 256) + color.b;
}