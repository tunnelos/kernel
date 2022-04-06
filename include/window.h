#pragma once

#include "./stdint.h"
#include "./smt.h"

typedef struct {
    //renderer parametres
    int wx;
    int wy;
    int sx;
    int sy;
    char *name;
    int color;

    //visual parametres
    bool can_be_closed;
    bool can_be_in_background;
    bool in_background;
    bool draw_border;
    bool rainbow;

    int id;
} window_t;

extern window_t *__window_windowlist[32];
extern bool __window_used[32];

void __window_create(window_t *window_settings);
void __window_remove(int id);

void __window_renderer(int id);
void __window_init();
void __window_save_context(char *old_context, window_t *new_context);
void __window_read_old_ctx(char *old_context, window_t *crt_context);