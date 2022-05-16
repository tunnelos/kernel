#include "./include/window_welcome.h"
#include "./include/screen.h"
#include "./include/mm.h"
#include "./include/stdio.h"

window_t __wWelcome_context;
char *__wWelcome_old_context;

void __wWelcome_update_handler() {
    puts_gui("Welcome to Tunnel OS. This a multicore", 0, __wWelcome_context.wx + 2, __wWelcome_context.wy + 2);
    puts_gui("text-based 64-bit Operating System without", 0, __wWelcome_context.wx + 2, __wWelcome_context.wy + 3);
    puts_gui("using paging nor PIT interrupts for switching", 0, __wWelcome_context.wx + 2, __wWelcome_context.wy + 4);
    puts_gui("tasks.", 0, __wWelcome_context.wx + 2, __wWelcome_context.wy + 5);
}
void __wWelcome_exit_handler() {
    __window_read_old_ctx(__wWelcome_old_context, &__wWelcome_context);
    uint8_t g = 0;
    while(g < 32) {
        if(__window_windowlist[g]) {
            if(__window_windowlist[g]->id == __wWelcome_context.id) {
                __window_windowlist[g] = 0;
                __window_used[g] = false;
            }
        }
        g++;
    }
}

void __wWelcome_start() {
    __wWelcome_old_context = (char *)(malloc((50 * 8) * (16 * 16)));
    __wWelcome_context.can_be_closed = true;
    __wWelcome_context.draw_border = true;
    __wWelcome_context.name = "Tunnel OS";
    __wWelcome_context.sx = 50;
    __wWelcome_context.sy = 16;
    __wWelcome_context.wx = 8;
    __wWelcome_context.wy = 5;
    __wWelcome_context.ctx0 = __wWelcome_old_context;
    __wWelcome_context.update_handler = __wWelcome_update_handler;
    __wWelcome_context.exit_handler = __wWelcome_exit_handler;

    //save framebuffer data before drawing window contents
    __window_save_context(__wWelcome_old_context, &__wWelcome_context);

    puts_gui("Welcome to Tunnel OS. This a multicore", 0, __wWelcome_context.wx + 2, __wWelcome_context.wx + 2);
    puts_gui("text-based 64-bit Operating System without", 0, __wWelcome_context.wx + 2, __wWelcome_context.wx + 3);
    puts_gui("using paging nor PIT interrupts for switching", 0, __wWelcome_context.wx + 2, __wWelcome_context.wx + 4);
    puts_gui("tasks.", 0, __wWelcome_context.wx + 2, __wWelcome_context.wx + 5);
    __wWelcome_context.color = COLOR_BLUE + COLOR_GREEN;
    __wWelcome_context.updated = true;
    __window_create(&__wWelcome_context);
}