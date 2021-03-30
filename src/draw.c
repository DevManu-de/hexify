#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "draw.h"
#include "global.h"

static WINDOW *title;
static WINDOW *text;
static WINDOW *divider;
static WINDOW *hex;
static WINDOW *lines;

static int characters_drawn;
static int hex_per_line;

void gui_init() {

    int maxx;
    int maxy;

    initscr();
    cbreak();

    getmaxyx(stdscr, maxy, maxx);
    /* Calculate hex and text ratio */
    int split = maxx * 0.3;

    unsigned int title_height = 3;
    unsigned int title_width = maxx;
    unsigned int title_y = 0;
    unsigned int title_x = 0;
    title = newwin(title_height, title_width, title_y, title_x);
    box(title, 0, 0);
    REFRESH_WINDOW(title)

    unsigned int text_height = maxy - title_height - 2;
    unsigned int text_width = split - 1;
    unsigned int text_y = title_height + 1;
    unsigned int text_x = maxx - split + 1;
    text = newwin(text_height, text_width, text_y, text_x);
    REFRESH_WINDOW(text)

    unsigned int divider_height = maxy - title_height;
    unsigned int divider_width = 2;
    unsigned int divider_y = title_height;
    unsigned int divider_x = maxx - split - 2;
    divider = newwin(divider_height, divider_width, divider_y, divider_x);
    box(divider, 0, 0);
    REFRESH_WINDOW(divider)

    unsigned int lines_height = maxy - title_height;
    unsigned int lines_width = 13;
    unsigned int lines_y = title_height;
    unsigned int lines_x = 0;
    lines = newwin(lines_height, lines_width, lines_y, lines_x);
    box(lines, ' ', 0);
    REFRESH_WINDOW(lines)

    unsigned int hex_height = maxy - title_height - 2;
    unsigned int hex_width = maxx - split - divider_width - lines_width - 1;
    unsigned int hex_y = title_height + 1;
    unsigned int hex_x = lines_width + 1;
    hex = newwin(hex_height, hex_width, hex_y, hex_x);
    REFRESH_WINDOW(hex)

}

void gui_draw_title(char *format, ...) {

    va_list ap;
    va_start(ap, format);
    wmove(title, 1, 1);
    vw_printw(title, format, ap);
    va_end(ap);
    REFRESH_WINDOW(title)

}

void gui_draw_hex(byte *file, unsigned int file_current_offset) {
    
    wclrtobot(hex);
    characters_drawn = 0;
    file += file_current_offset;
    hex_per_line = getmaxx(hex) / 3;
    unsigned int file_max_drawable = hex_per_line * getmaxy(hex);
    unsigned int file_size = strlen(file);
    unsigned int file_draw_size = (file_max_drawable < file_size) ? file_max_drawable : file_size;
    unsigned int line = 0;
    unsigned int i;
    wmove(hex, 0, 0);
    wmove(text, 0, 0);
    for (i = 0; i < file_draw_size; i += hex_per_line, ++line) {
        for (int j = 0; (j < hex_per_line) && (i + j < file_draw_size); ++j) {
            wprintw(hex, "%02x ", file[i+j]);
            ++characters_drawn;
            switch (file[i+j]) {
                case '\n':
                    wprintw(text, "\\n");
                    continue;
                case '\t':
                    wprintw(text, "\\t");
                    continue;
            }
            wprintw(text, "%c", file[i+j]);
        }    
        wprintw(text, "\n");
        mvwprintw(lines, line, 1, "0x%08x:", i + file_current_offset);

    }
    mvwprintw(lines, line, 1, "0x%08x:", i + file_current_offset);

    box(lines, ' ', 0);
    REFRESH_WINDOW(hex)
    REFRESH_WINDOW(text)
    REFRESH_WINDOW(lines)

}

void draw_cursor_reset(unsigned int *file_current_offset) {
    
    wmove(hex, 0, 0);
    REFRESH_WINDOW(hex)
}

void draw_cursor_up(unsigned int *file_current_offset, byte *file) {

    unsigned int maxy = getmaxy(hex);
    unsigned int cur_y = getcury(hex);
    unsigned int cur_x = getcurx(hex);
    if (0 == cur_y && *file_current_offset > 0) {
        *file_current_offset -= hex_per_line;
        gui_draw_hex(file, *file_current_offset);
        wmove(hex, cur_y, cur_x);
    } else {
        wmove(hex, cur_y - 1, cur_x);
    }
    REFRESH_WINDOW(hex)
}

void draw_cursor_down(unsigned int *file_current_offset, byte *file) {

    int maxy = getmaxy(hex);
    int cur_y = getcury(hex);
    int cur_x = getcurx(hex);

    int down_max_count = strlen(file) / hex_per_line - ((int) *file_current_offset) / hex_per_line - getmaxy(hex);

    if (maxy - 1 == cur_y && down_max_count >= 0) {
        *file_current_offset += hex_per_line;
        gui_draw_hex(file, *file_current_offset);
        wmove(hex, cur_y, cur_x);
    } else if (cur_y < characters_drawn / hex_per_line) {

        wmove(hex, cur_y + 1, cur_x);
    }
    REFRESH_WINDOW(hex)
}
void draw_cursor_right(unsigned int *file_current_offset, byte *file) {

    int last_row_caracter_count = (characters_drawn % hex_per_line);
    last_row_caracter_count = (last_row_caracter_count == 0) ? hex_per_line : last_row_caracter_count;
    int cur_y = getcury(hex);
    int cur_x = getcurx(hex);

    if (cur_x <= last_row_caracter_count * 3 - 2) {
        wmove(hex, cur_y, cur_x + 1);
        REFRESH_WINDOW(hex)
    }
}
void draw_cursor_left(unsigned int *file_current_offset, byte *file) {

    wmove(hex, getcury(hex), getcurx(hex)-1);
    REFRESH_WINDOW(hex)
}

void gui_end() {
    
    endwin();

}
