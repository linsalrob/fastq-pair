//
// Created by redwards on 7/29/17.
//

#ifndef CEEQLIB_INDEX_FASTQ_H
#define CEEQLIB_INDEX_FASTQ_H

#include <stdbool.h>


/*
 * idloc is a struct with the current file position (pos) from ftell,
 * the id string for the sequence, and whether or not we've printed it out.
 * next is a pointer to the next idloc element in the hash.
 */
struct idloc {
    long int pos;
    char *id;
    bool printed;
    struct idloc *next;
};


/*
 * options are our options that can be passed in. The most important
 * is the table size.
 */

struct options {
    int tablesize;
    bool print_table_counts;
    bool verbose;
};

// how long should our lines be. This is a 64k buffer
#define MAXLINELEN 65536



/*
 * calculate the hash for a fastq sequence
 *
 * This is a simple hash but widely used!
 *
 * we use an unsigned here so that the answer is > 0
 *
 * You still need to mod this on the table size
 */

unsigned hash (char *s);
/*
 * Take two fastq files (f and g), we generate paired output.
 * t is the tablesize and is the most important parameter
 */
int pair_files(char *f, char *g, struct options *o);

#endif //CEEQLIB_INDEX_FASTQ_H
