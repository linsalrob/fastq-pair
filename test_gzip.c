//
// Created by Rob Edwards on 8/5/19.
// Test the gzip checking code
// To compile this code, you can just use: gcc -std=gnu99  -o testgz ./test_gzip.c  is_gzipped.c
//
#include "is_gzipped.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stdout, "%s <file to test>\n", argv[0]);
        exit(-1);
    }

    if (test_gzip(argv[1])) {
        fprintf(stdout, "%s IS gzip compressed\n", argv[1]);
    } else {
        fprintf(stdout, "%s is NOT gzip compressed\n", argv[1]);
    }
}