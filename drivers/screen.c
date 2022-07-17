#include "../include/screen.h"
#include "../include/color.h"

/*********** SCREEN FUNCTIONS ***********/

void __screen_write_pixel(int pos_x, int pos_y, uint32_t color){
    uint32_t *loc = ((uint32_t *)(&fb + scanlines * pos_y + pos_x * 4));
    *loc = color;
}