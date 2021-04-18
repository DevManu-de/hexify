#include <getopt.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"
#include "file.h"
#include "global.h"
#include "xmemtools.h"

void help();
void version();

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        help();
    }

    char *file_name = strdup(argv[1]);
    float ratio = 0.3;

    static struct option long_options[] = {
        {"file", required_argument, NULL, 'f'},
        {"ratio", required_argument, NULL, 'r'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'}

    };

    int opts;
    while ((opts = getopt_long(argc, argv, "f:r:hv", long_options, NULL)) != -1) {

        switch (opts) {

            case 'f':
                xfree(file_name);
                file_name = strdup(optarg);
                break;
            case 'r':
                ratio = strtof(optarg, NULL);
                if (ratio <= 0 || ratio >= 1) {
                    die(RATERR, "ratio (%s) must be betweem 0 and 1", optarg);
                }
                break;
            case 'h':
                help();
                break;
            case 'v':
                version();
                break;
            default:
                die(ARGERR, NULL);
                break;

        }

    }

    /* file_size is the amount of bytes of the file */
    size_t file_size;

    /* file_current_offset is the amount of bytes from where the file is drawn on screen */
    size_t file_current_offset = 0;

    /* file_content is the byte array containin the entire file */
    byte *file_content = file_name_get_content(file_name, &file_size);

    /* Start gui and draw file name to title */
    gui_init(ratio);
    gui_draw_title("Opened file: %s", file_name);

    /* Draw the byte count hex numbers and ascii caracters */
    gui_draw_hex(file_content, file_current_offset, file_size);

    /* Set the cursor to the start */
    draw_cursor_reset();

    int inp;
    while ((inp = getchar()) != 'q') {

        switch (inp) {

            case -1:
                /* trigger a refresh when terminal got resized */
                gui_init(ratio);
                gui_draw_hex(file_content, file_current_offset, file_size);
                gui_draw_title("Open file: %s", file_name);
                break;

            case 27:
                getchar();
                inp = getchar();
                switch (inp) {
                    
                    case 'A':
                        /* Arrow up */
                        draw_cursor_up(&file_current_offset, file_content, file_size);
                        break;
                    case 'B':
                        /* Arrow down */
                        draw_cursor_down(&file_current_offset, file_content, file_size);
                        break;
                    case 'C':
                        /* Arrow right */
                        draw_cursor_right(&file_current_offset, file_content, file_size);
                        break;
                    case 'D':
                        /* Arrow left */
                        draw_cursor_left(&file_current_offset, file_content, file_size);
                        break;

                }
        }
    }

    gui_end();
    xfree(file_name);
    xfree(file_content);

	return 0;
}

void help() {

    printf("hexify help page\n");
    exit(0);
}

void version() {
    
    printf("hexify: %s\n", VERSION);
    exit(0);
}

void die(enum errcodes errcode, const char *format, ...) {
    gui_end();
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    if (format != NULL) {
        putc('\n', stderr);
    }
    va_end(ap);
    exit(errcode);
}

