#include "../include/stdint.h"
#include "../include/screen.h"
#include "../include/ui.h"
#include "../include/stdio.h"


ui_col_t __ui_colors;

void __ui_init(){
    __ui_colors.primary = COLOR_GREEN;
    __ui_colors.secondary = COLOR_LIGHT_GRAY;
    __ui_colors.tertiary = COLOR_GRAY;
}

void __ui_window_create(uint32_t color, int x, int y, int width, int height) {
    for (int i = y; i < height; i++) {
        for (int j = x; j < width; i++) {
            putc('\x08', color, i, j);

            if ((i - y) == 1 && (j - x) == width) {
                putc('X', COLOR_BLACK, i, j);
            }
        }
    }
} 

void __ui_window_init() {
    for(int y = 1; y < SCREEN_HEIGHT; y++){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            putc('\x08', __ui_colors.primary, x, y);
        }
        putc('x', COLOR_BLACK, 79, 0);
    }
}