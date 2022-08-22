#pragma once

#include "./screen.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    /* data */
} ui_t;

typedef struct {

} ui_dev_t;
typedef struct {

} ui_theme_t;
typedef struct {
    int primary;
    int secondary;
    int tertiary;
} ui_col_t;

extern ui_col_t __ui_colors;

#ifdef __cplusplus
}
#endif