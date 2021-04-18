#ifndef _DRAW_H
#define _DRAW_H

#include <stdlib.h>

#include "global.h"

#define REFRESH_WINDOW(win) refresh();              \
                            wrefresh(win);          \

void gui_init(float ratio);
void gui_draw_title(const char *format, ...);
void gui_draw_hex(byte *file, size_t file_current_offset, size_t file_size);
void draw_cursor_reset();
void draw_cursor_up(size_t *file_current_offset, byte *file, size_t file_size);
void draw_cursor_down(size_t *file_current_offset, byte *file, size_t file_size);
void draw_cursor_right(size_t *file_current_offset, byte *file, size_t file_size);
void draw_cursor_left(size_t *file_current_offset, byte *file, size_t file_size);
void gui_end();

#endif
