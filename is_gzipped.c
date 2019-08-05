//
// Created by Rob Edwards on 8/5/19.
// Test whether a file is gzip compressed and return 1 (true) for compressed and 0 (false) for uncompressed
//

#include <stdio.h>
#include <stdbool.h>
#include "is_gzipped.h"

bool test_gzip(char* filename) {
    FILE *fileptr;
    char buffer[2];

    fileptr = fopen(filename, "rb");
    fread(buffer, 2, 1, fileptr);
    if ((buffer[0] == '\x1F') && (buffer[1] == '\x8B'))
        return true;

    return false;
}
