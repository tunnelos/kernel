#pragma once

#include "flags.h"

#pragma pack(push, 1)
typedef struct {
    int x;
    int y;
} vector2d_t;

enum DrawTask {
    Rectangle = 0,
    Text = 1,
    Pixel = 2
};
enum DrawState {
    Idle = 0,
    Unused = 1,
    Active = 2
};

typedef struct {
    vector2d_t pos;
    vector2d_t size;
    
    char *text;
    int color;

    enum DrawTask task;
    enum DrawState state;
} drawtask_t;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

extern drawtask_t *__coreshell_drawtasks;

#if INTERNAL_FUNCS_IN_HEADERS == 1  
// Core 2
void __coreshell_init_coreRenderer();
// Core 1
void __coreshell_init_coreIOHandler();
// Core 0
void __coreshell_init_coreExecuter();
#endif

void __coreshell_init_all();

#ifdef __cplusplus
}
#endif