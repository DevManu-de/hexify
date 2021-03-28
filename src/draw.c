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
    hex = newwin(hex_height, hex_width, title_height, hex_x);
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

void gui_draw_hex(byte *file) {
    
    size_t hex_per_line = getmaxx(hex) / 3;
    unsigned int file_max_drawable = hex_per_line * getmaxy(hex);
    unsigned int file_size = strlen(file);
    unsigned int file_draw_size = (file_max_drawable < file_size) ? file_max_drawable : file_size;
    unsigned int line = 0;
    for (unsigned int i = 0; i < file_draw_size; i += hex_per_line, ++line) {
        for (unsigned int j = 0; (j < hex_per_line) && (i + j < file_draw_size); ++j) {
            switch (file[i+j]) {
                case '\n':
                    wprintw(text, "\\n");
                    continue;
                case '\t':
                    wprintw(text, "\\t");
                    continue;
            }
            wprintw(hex, "%02x ", file[i+j]);
            wprintw(text, "%c", file[i+j]);
        }    
        wprintw(hex, "\n");
        wprintw(text, "\n");
        mvwprintw(lines, line, 1, "0x%08x", i + hex_per_line);

    }

    box(lines, ' ', 0);
    REFRESH_WINDOW(hex)
    REFRESH_WINDOW(text)
    REFRESH_WINDOW(lines)

}

void gui_end() {
    
    endwin();

}
