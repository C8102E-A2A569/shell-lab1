#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../include/io-lat-read/tools.h"

size_t parse_size(const char *size_str) {
    char *endptr;
    errno = 0;
    size_t size = strtoull(size_str, &endptr, 10);
    if (errno != 0 || *endptr != '\0') {
        fprintf(stderr, "Invalid size argument: %s\n", size_str);
        exit(EXIT_FAILURE);
    }
    return size;
}
