#ifndef _GLOBAL_H
#define _GLOBAL_H

#define VERSION "0.2.0"

typedef char byte;

enum errcodes {

    SUCCESS,
    ARGERR,
    RATERR,
    FILEERR

};

void die(enum errcodes errcode, const char *format, ...);

#endif
