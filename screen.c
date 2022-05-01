#include "./include/screen.h"
#include "./include/color.h"

uint32_t __screen_draw_x1 = 0;
uint32_t __screen_draw_x2 = SCREEN_WIDTH; 
uint32_t __screen_draw_y1 = 0; 
uint32_t __screen_draw_y2 = SCREEN_HEIGHT;

uint32_t *__screen_draw_array[4] = {
    &__screen_draw_x1,
    &__screen_draw_x2,
    &__screen_draw_y1,
    &__screen_draw_y2
};

/*********** DRAW LIMIT ***********/

void __screen_set_draw_limit(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2){
    __screen_draw_x1 = x1, __screen_draw_x2 = x2, __screen_draw_y1 = y1, __screen_draw_y2 = y2;
    return;
}

void __screen_reset_draw_limit() {
    __screen_draw_x1 = 0;
    __screen_draw_x2 = SCREEN_WIDTH; 
    __screen_draw_y1 = 0; 
    __screen_draw_y2 = SCREEN_HEIGHT;
}

uint32_t **__screen_get_draw_limit(){
    return __screen_draw_array;
}

/*********** SCREEN FUNCTIONS ***********/

void __screen_flush(int width, int height){
    if(width == 0 || height == 0){
        width = SCREEN_WIDTH;
        height = SCREEN_HEIGHT;
    }
    
    __screen_reset_draw_limit();

    for(uint32_t y = 0; y < SCREEN_HEIGHT; y++){
        for(uint32_t x = 0; x < SCREEN_WIDTH; x++){
            __screen_write_pixel(x, y, COLOR_BLACK);
        }
    }
    
    return;
}

void __screen_write_pixel(int pos_x, int pos_y, uint32_t color){
    uint32_t *loc = ((uint32_t *)(&fb + scanlines * pos_y + pos_x * 4));
    *loc = color;
}

void __screen_draw_rectangle(int x, int y, int width, int height, uint32_t color, int transparency){
    int temp;

    //Лево
    if(x < __screen_draw_x1){
        width = width - __screen_draw_x1 + x;
        x = __screen_draw_x1;
    }

    // Право
    temp = x + width - 1;
    if(temp > __screen_draw_x2){
        width = width - temp + __screen_draw_x2;
    }

    // Верх
    if(y < __screen_draw_y1){
        height = height - __screen_draw_y1 + y;
        y = __screen_draw_y1;
    }

    // Низ
    temp = y + height - 1;
    if(temp > __screen_draw_y2){
        height = height - temp + __screen_draw_y2;
    }

    temp = SCREEN_WIDTH * (y - 1) + x;

    if(transparency == 0){
        for (int j = 1; j < height; j++){
            for(int i = 1; i < width; i++){
                __screen_write_pixel(i, j, color);
            }
        }
    } else {
        for (int j = 1; j < height; j++){
            for(int i = 1; i < width; i++){
                if(color != 0) __screen_write_pixel(i, j, color);
            }
        }
    }
}