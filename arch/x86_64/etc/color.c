#include "../include/color.h"

uint32_t __color_to_int(color_t color){
    return *((uint32_t *)&color);
}