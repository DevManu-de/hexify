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

/* Characters_drawn is a variable that updated every time gui_draw_hex is ran
 * it contains the amount of characters that were drawn 1 hex number is 1 character drawn */
static int characters_drawn;
/* Contains the amount of hex numbers that are in one row */
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

void gui_draw_hex(byte *file, long file_current_offset, unsigned long file_size) {
    
    /* Reset characters_drawn */
    characters_drawn = 0;
    /* File += file_current_offset determines which part the user gets displayed */
    file += file_current_offset;
    /* Update hex_per_line */
    hex_per_line = getmaxx(hex) / 3;
    /* File_max_drawable is the amount of hex numbers that fit in the current screen */
    int file_max_drawable = hex_per_line * getmaxy(hex);
    /* File_draw_size determines how many character should be displayed */
    int file_draw_size = (file_max_drawable < file_size) ? file_max_drawable : file_size;
    int line = 0;
    /* Reset the cursor in both windows */
    wmove(hex, 0, 0);
    wmove(text, 0, 0);
    /* Clear hex from previous text */
    wclrtobot(hex);
    int i; /* Jumps from line to line */
    for (i = 0; i < file_draw_size; i += hex_per_line, ++line) {
        /* Prints the characters in and byte numbers */
        for (int j = 0; (j < hex_per_line) && (i + j < file_draw_size); ++j) {
            /* Display the hex numbers */
            wprintw(hex, "%02x ", file[i+j]);
            ++characters_drawn;
            /* Determine if an tab or line break character was encountered */
            switch (file[i+j]) {
                case '\n':
                    wprintw(text, "\\n");
                    continue;
                case '\t':
                    wprintw(text, "\\t");
                    continue;
                case '\r':
                    wprintw(text, "\\r");
                    continue;
            }
            /* Print the ascii character */
            wprintw(text, "%c", file[i+j]);
        }
        /* Print a newline at the text window */
        wprintw(text, "\n");
        /* Display the byte numbers */
        mvwprintw(lines, line, 1, "0x%08x:", i + file_current_offset);

    }
    /* Print the final line because its out of the loop */
    mvwprintw(lines, line, 1, "0x%08x:", i + file_current_offset);

    /* Redraw the box arount the byte numbers */
    box(lines, ' ', 0);
    REFRESH_WINDOW(hex)
    REFRESH_WINDOW(text)
    REFRESH_WINDOW(lines)

}

void draw_cursor_reset(long *file_current_offset) {
    
    wmove(hex, 0, 0);
    REFRESH_WINDOW(hex)
}

void draw_cursor_up(long *file_current_offset, byte *file, unsigned long file_size) {

    int cur_y = getcury(hex);
    int cur_x = getcurx(hex);
    /* Only go up if there is content */
    if (0 == cur_y && *file_current_offset > 0) {
        *file_current_offset -= hex_per_line;
        gui_draw_hex(file, *file_current_offset, file_size);
        wmove(hex, cur_y, cur_x);
    } else {
        wmove(hex, cur_y - 1, cur_x);
    }
    REFRESH_WINDOW(hex)
}

void draw_cursor_down(long *file_current_offset, byte *file, unsigned long file_size) {

    /* last_row_caracter_count is the amout of hex numbers in the last line that is on the screen visible */
    int last_row_caracter_count = (characters_drawn % hex_per_line);
    /* row_count is the amount of lines that were drawn on screen */
    int row_count = characters_drawn / hex_per_line;
    int maxy = getmaxy(hex);
    int cur_y = getcury(hex);
    int cur_x = getcurx(hex);

    /* down_max_count determines how many time the user can press arrow down */
    int down_max_count = file_size / hex_per_line - ((int) *file_current_offset) / hex_per_line - maxy;

    /* If cursor is 1 above the last line and beyond the hex numbers */
    if (cur_x >= last_row_caracter_count * 3 && cur_y == row_count - 1 && down_max_count < 0) {
    /* last_row_caracter_count must be multiplied with 3 because one hex number is 2 characters and 1 space */
        wmove(hex, cur_y + 1, last_row_caracter_count * 3 - 1);
    } else if (maxy - 1 == cur_y && down_max_count >= 0) {
        /* Prevent user to go beyond the file */
        *file_current_offset += hex_per_line;
        gui_draw_hex(file, *file_current_offset, file_size);
        wmove(hex, cur_y, cur_x);
    } else if (cur_y < characters_drawn / hex_per_line) {
        /* Move the cursor one down */
        wmove(hex, cur_y + 1, cur_x);
    }
    REFRESH_WINDOW(hex)
}
void draw_cursor_right(long *file_current_offset, byte *file, unsigned long file_size) {

    /* last_row_caracter_count is the amout of hex numbers in the last line that is on the screen visible */
    int last_row_caracter_count = (characters_drawn % hex_per_line);
    /* row_count is the amount of lines that were drawn on screen */
    int row_count = characters_drawn / hex_per_line;
    /* If last_row_caracter_count is 0 then the line is completely filled with hex numbers */
    last_row_caracter_count = (last_row_caracter_count == 0) ? hex_per_line : last_row_caracter_count;
    int cur_y = getcury(hex);
    int cur_x = getcurx(hex);

    /* last_row_caracter_count must be multiplied with 3 because one hex number is 2 characters and 1 space */
    if (cur_x <= last_row_caracter_count * 3 - 2 || cur_y < row_count) {
        wmove(hex, cur_y, cur_x + 1);
        REFRESH_WINDOW(hex)
    }
}
void draw_cursor_left(long *file_current_offset, byte *file, unsigned long file_size) {

    wmove(hex, getcury(hex), getcurx(hex)-1);
    REFRESH_WINDOW(hex)
}

void gui_end() {
    
    endwin();

}
