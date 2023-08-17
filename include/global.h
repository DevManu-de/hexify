#ifndef _GLOBAL_H
#define _GLOBAL_H

#define VERSION "0.2.2"

typedef unsigned char byte;

enum errcodes {

    /* SUCCESS when exit was expected */
    SUCCESS,
    /* ARGERR when argument parsing failed */
    ARGERR,
    /* RATERR when ratio failed */
    RATERR,
    /* FILEERR when file open/read failed */
    FILEERR

};

void die(enum errcodes errcode, const char *format, ...);

#endif
