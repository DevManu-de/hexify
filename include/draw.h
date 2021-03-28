#ifndef _DRAW_H
#define _DRAW_H

#include <stdlib.h>

#include "global.h"

#define REFRESH_WINDOW(win) refresh();              \
                            wrefresh(win);          \

void gui_init();
void gui_draw_title(char *format, ...);
void gui_draw_hex(byte *file, unsigned int file_current_offset);
void draw_cursor_reset(unsigned int *file_current_offset);
void draw_cursor_up(unsigned int *file_current_offset, byte *file);
void draw_cursor_down(unsigned int *file_current_offset, byte *file);
void draw_cursor_right(unsigned int *file_current_offset, byte *file);
void draw_cursor_left(unsigned int *file_current_offset, byte *file);
void gui_end();

#endif
