#include "./include/window_welcome.h"
#include "./include/screen.h"
#include "./include/mm.h"
#include "./include/stdio.h"

window_t __wWelcome_context;
char *__wWelcome_old_context;

void __wWelcome_start() {
    
    __wWelcome_old_context = (char *)(malloc((50 * 8) * (16 * 16)).address);
    //putc('!', COLOR_RED, 10, 7);
    __wWelcome_context.can_be_closed = true;
    //__wWelcome_context.can_be_in_background = true;
    __wWelcome_context.draw_border = true;
    __wWelcome_context.name = "Tunnel OS";
    __wWelcome_context.sx = 50;
    __wWelcome_context.sy = 16;
    __wWelcome_context.wx = 8;
    __wWelcome_context.wy = 5;
    //save framebuffer data before drawing window contents
    __window_save_context(__wWelcome_old_context, &__wWelcome_context);

    puts_gui("Welcome to Tunnel OS. This a multicore", 0, 10, 7);
    puts_gui("text-based Operating System without using", 0, 10, 8);
    puts_gui("paging nor PIT interrupts for switching", 0, 10, 9);
    puts_gui("tasks.", 0, 10, 10);
    //__window_read_old_ctx(__wWelcome_old_context, &__wWelcome_context);
    __wWelcome_context.rainbow = false;
    __wWelcome_context.color = COLOR_BLUE + COLOR_GREEN;
    __window_create(&__wWelcome_context);
}