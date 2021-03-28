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

    char *file_name;

    static struct option long_options[] = {
        {"file", required_argument, NULL, 'f'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'}

    };

    int opts;
    while ((opts = getopt_long(argc, argv, "f:hv", long_options, NULL)) != -1) {

        switch (opts) {

            case 'f':
                file_name = strdup(optarg);
                break;
            case 'h':
                help();
                break;
            case 'v':
                version();
                break;

        }

    }

    size_t file_size;
    byte *file_content = file_name_get_content(file_name, &file_size);

    gui_init();
    gui_draw_title("THIS IS A TEST TITLE");

    gui_draw_hex(file_content);

    int inp;
    while ((inp = getchar()) != 'q') {
        if (inp == -1) {
            doupdate();
        }
    }

    gui_end();
    xfree(file_name);
    xfree(file_content);

	return 0;
}

void help() {

    printf("hexify help page");
    exit(0);
}
void version() {
    
    printf("hexify: %s", VERSION);
    exit(0);

}

