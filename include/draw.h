#ifndef _DRAW_H
#define _DRAW_H

#include <stdlib.h>

#include "global.h"

#define REFRESH_WINDOW(win) refresh();              \
                            wrefresh(win);          \

void gui_init();
void gui_draw_title(char *format, ...);
void gui_draw_hex(byte *file, long file_current_offset, unsigned long file_size);
void draw_cursor_reset(long *file_current_offset);
void draw_cursor_up(long *file_current_offset, byte *file, unsigned long file_size);
void draw_cursor_down(long *file_current_offset, byte *file, unsigned long file_size);
void draw_cursor_right(long *file_current_offset, byte *file, unsigned long file_size);
void draw_cursor_left(long *file_current_offset, byte *file, unsigned long file_size);
void gui_end();

#endif
