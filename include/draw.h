#ifndef _DRAW_H
#define _DRAW_H

#include <stdlib.h>

#include "global.h"

#define REFRESH_WINDOW(win) refresh();              \
                            wrefresh(win);          \

void gui_init();
void gui_draw_title(char *format, ...);
void gui_draw_hex(byte *file);
void draw_cursor_reset();
void draw_cursor_up();
void draw_cursor_down();
void draw_cursor_right();
void draw_cursor_left();
void gui_end();

#endif
