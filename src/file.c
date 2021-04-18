#include <stdio.h>
#include <stdlib.h>

#include "draw.h"
#include "global.h"
#include "xmemtools.h"


byte *file_name_get_content(char *file_name, size_t *size) {

    FILE *file = fopen(file_name, "rb");
    if (!file) {
        die(FILEERR, "File: %s not found", file_name);
    }
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    byte *file_content = xmalloc(*size);
    fread(file_content, *size, sizeof(byte), file);
    fclose(file);

    return file_content;

}
