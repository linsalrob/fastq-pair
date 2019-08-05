#include "fastq_pair.h"
#include "is_gzipped.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>

long long get_time_ms()
{
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}


void help(char *s);

int main(int argc, char* argv[]) {

    if (argc == 2 && (strcmp(argv[1], "-V") == 0)) {
        fprintf(stdout, "%s version %0.1f\n", argv[0], 0.3);
        exit(0);
    }
    if (argc < 3) {
        help(argv[0]);
        exit(0);
    }

    struct options *opt;
    opt = malloc(sizeof(struct options));

    opt->tablesize = 100003;
    opt->print_table_counts = false;
    opt->verbose = false;
    char *left_file = NULL;
    char *right_file = NULL;

    // access from unistd.h is a method to check whether a file exists.
    // we use this to parse the file name and see if it is a valid file.

    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "-t") == 0)
            opt->tablesize = atoi(argv[++i]);
        else if (strcmp(argv[i], "-p") == 0)
            opt->print_table_counts = true;
        else if (strcmp(argv[i], "-v") == 0)
            opt->verbose = true;
        else if (access(argv[i], F_OK) != -1 && left_file == NULL)
            left_file = argv[i];
        else if (access(argv[i], F_OK) != -1 && right_file == NULL)
            right_file = argv[i];
        else
            fprintf(stderr, "\n\nERROR: Not sure what parameter %s is\n", argv[i]);
    }

    if (left_file == NULL || right_file == NULL) {
        fprintf(stderr, "\n\nERROR: We could not find the files you specified.\n");
        help(argv[0]);
        exit(-1);
    }

    //if (test_gzip(left_file) || test_gzip(right_file)) {
    if (test_gzip(left_file) || test_gzip(right_file)) {
        fprintf(stderr, "ERROR: It appears that your files are compressed with gzip.\n");
        fprintf(stderr, "At this time we can't handle gzipped files because we use random access reading of the data stream.\n");
        fprintf(stderr, "Please either uncompress the files, or check these alternatives: https://edwards.sdsu.edu/research/sorting-and-paring-fastq-files/\n");
        exit(-1);
    }


    long long start_time, end_time, overhead_time;
    start_time = get_time_ms();
    end_time = get_time_ms();
    overhead_time = end_time - start_time;

    start_time = get_time_ms();
    int success = pair_files(left_file, right_file, opt);
    end_time = get_time_ms();
    if (opt->verbose)
        printf ("Elapsed time = %lld (ms)\n", end_time - start_time - overhead_time);

    return success;
}

void help(char *s) {
    fprintf(stdout, "\n%s [options] [fastq file 1] [fastq file 2]\n", s);
    fprintf(stdout, "\nOPTIONS\n-t table size (default 100003)\n");
    fprintf(stdout, "-p print the number of elements in each bucket in the table\n");
    fprintf(stdout, "-v verbose output. This is mainly for debugging\n");
    fprintf(stdout, "-V print the current version number and exit\n");
}

