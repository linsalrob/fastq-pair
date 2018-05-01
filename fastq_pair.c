/*
 * Pair a fastq file using a quick index.
 *
 * We have two files, and each file holds sequence data. Each data element is represented by only four, and exactly four
 * lines:
 *
 * @id1/1    <- the sequence identifier always begins with an @ sign and then the identifier is the string upto the first whitespace. The identifier must be unique per sequence in a file
 * ATGATC.... <- the DNA sequence
 * +        <- a line that begins with the + sign and may also have the identifier (but doesn't need to!)
 * !%$#^@     <- an ascii representation of the "quality" of the sequence. The quality is a number between 0 and 100 and this is the chr of that number +33
 *
 * In the two files, there should be some sequences that are related, and they are denoted either as 1/2,  forward/reverse, or just by having the whole id the same
 *
 * @id1/1  pairs with @id1/2
 *
 * or
 *
 * @id1/f pairs with @id1/r
 *
 * or
 *
 * @id1 in file 1 pairs with @id1 in file 2
 *
 * We read the file and make a hash of the ID without the /1 or /f and the position of that id in the file (using tell)
 * Then we read the second file and check to see if we have a matching sequence. If we do, we print both sequences
 * one to each file, and we set the "printed" boolean to true in our data structure corresponding to that ID.
 *
 * Finally, we read through our data structure and print out any sequences that we have not seen before.
 *
 * Note that to print out the sequences we seek to the position we recorded and print four lines.
 *
 */

#include "fastq_pair.h"
#include "robstr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pair_files(char *left_fn, char *right_fn, struct options *opt) {

    // our hash for the fastq ids.
    struct idloc **ids;
    ids = malloc(sizeof(*ids) * opt->tablesize);

    // if we are not able to allocate the memory for this, there is no point continuing!
    if (ids == NULL) {
        fprintf(stderr, "We cannot allocate the memory for a table size of %d. Please try a smaller value for -t\n", opt->tablesize);
        exit(-1);
    }

    FILE *lfp;

    char *line = malloc(sizeof(char) * MAXLINELEN + 1);

    if ((lfp = fopen(left_fn, "r")) == NULL) {
        fprintf(stderr, "Can't open file %s\n", left_fn);
        exit(1);
    }

    char *aline; /* this variable is not used, it suppresses a compiler warning */

    long int nextposition = 0;

    /*
     * Read the first file and make an index of that file.
     */
    while ((aline = fgets(line, MAXLINELEN, lfp)) != NULL) {
        struct idloc *newid;
        newid = (struct idloc *) malloc(sizeof(*newid));

        if (newid == NULL) {
            fprintf(stderr, "Can't allocate memory for new ID pointer\n");
            return 0;
        }

        line[strcspn(line, "\n")] = '\0';
        line[strcspn(line, " ")] = '\0';

        /*
         * Figure out what the match mechanism is. We have three examples so
         *     i.   using /1 and /2
         *     ii.  using /f and /r
         *     iii. just having the whole name
         *
         * If there is a /1 or /2 in the file name, we set that part to null so the string is only up
         * to before the / and use that to store the location.
         */

        char lastchar = line[strlen(line)-1];
        char lastbutone = line[strlen(line)-2];
        if ('/' == lastbutone || '_' == lastbutone || '.' == lastbutone)
            if ('1' == lastchar || '2' == lastchar || 'f' == lastchar ||  'r' == lastchar)
                line[strlen(line)-1] = '\0';

        if (opt->verbose)
            fprintf(stderr, "ID is |%s|\n", line);

        newid->id = dupstr(line);
        newid->pos = nextposition;
        newid->printed = false;
        newid->next = NULL;

        unsigned hashval = hash(newid->id) % opt->tablesize;
        newid->next = ids[hashval];
        ids[hashval] = newid;

        /* read the next three lines and ignore them: sequence, header, and quality */
        for (int i=0; i<3; i++)
            aline = fgets(line, MAXLINELEN, lfp);

        nextposition = ftell(lfp);
    }


    /*
     * Now just print all the id lines and their positions
     */

    if (opt->print_table_counts) {
        fprintf(stdout, "Bucket sizes\n");

        for (int i = 0; i < opt->tablesize; i++) {
            struct idloc *ptr = ids[i];
            int counter=0;
            while (ptr != NULL) {
                // fprintf(stdout, "ID: %s Position %ld\n", ptr->id, ptr->pos);
                counter++;
                ptr = ptr->next;
            }
            fprintf(stdout, "%d\t%d\n", i, counter);
        }
    }

   /* now we want to open output files for left_paired, right_paired, and right_single */

    FILE * left_paired;
    FILE * left_single;
    FILE * right_paired;
    FILE * right_single;

    char *lpfn = catstr(dupstr(left_fn), ".paired.fq");
    char *rpfn = catstr(dupstr(right_fn), ".paired.fq");
    char *lsfn = catstr(dupstr(left_fn), ".single.fq");
    char *rsfn = catstr(dupstr(right_fn), ".single.fq");

    printf("Writing the paired reads to %s and %s.\nWriting the single reads to %s and %s\n", lpfn, rpfn, lsfn, rsfn);

    if ((left_paired = fopen(lpfn, "w")) == NULL ) {
        fprintf(stderr, "Can't open file %s\n", lpfn);
        exit(1);
    }

    if ((left_single = fopen(lsfn, "w")) == NULL) {
        fprintf(stderr, "Can't open file %s\n", lsfn);
        exit(1);
    }
    if ((right_paired = fopen(rpfn, "w")) == NULL) {
        fprintf(stderr, "Can't open file %s\n", rpfn);
        exit(1);
    }

    if ((right_single = fopen(rsfn, "w")) == NULL) {
        fprintf(stderr, "Can't open file %s\n", rsfn);
        exit(1);
    }



    /*
    * Now read the second file, and print out things in common
    */

    int left_paired_counter=0;
    int right_paired_counter=0;
    int left_single_counter=0;
    int right_single_counter=0;

    FILE *rfp;

    if ((rfp = fopen(right_fn, "r")) == NULL) {
        fprintf(stderr, "Can't open file %s\n", left_fn);
        exit(1);
    }

    nextposition = 0;

    while ((aline = fgets(line, MAXLINELEN, rfp)) != NULL) {

        // make a copy of the current line so we can print it out later.
        char *headerline = dupstr(line);

        line[strcspn(line, "\n")] = '\0';
        line[strcspn(line, " ")] = '\0';

        /* remove the last character, as we did above */

        char lastchar = line[strlen(line)-1];
        char lastbutone = line[strlen(line)-2];
        if ('/' == lastbutone || '_' == lastbutone || '.' == lastbutone)
            if ('1' == lastchar || '2' == lastchar || 'f' == lastchar ||  'r' == lastchar)
                line[strlen(line)-1] = '\0';

        // now see if we have the mate pair
        unsigned hashval = hash(line) % opt->tablesize;
        struct idloc *ptr = ids[hashval];
        long int posn = -1; // -1 is not a valid file position
        while (ptr != NULL) {
            if (strcmp(ptr->id, line) == 0) {
                posn = ptr->pos;
                ptr->printed = true;
            }
            ptr = ptr->next;
        }

        if (posn != -1) {
            // we have a match.
            // lets process the left file
            fseek(lfp, posn, SEEK_SET);
            left_paired_counter++;
            for (int i=0; i<=3; i++) {
                aline = fgets(line, MAXLINELEN, lfp);
                fprintf(left_paired, "%s", line);
            }
            // now process the right file
            fprintf(right_paired, "%s", headerline);
            right_paired_counter++;
            for (int i=0; i<=2; i++) {
                aline = fgets(line, MAXLINELEN, rfp);
                fprintf(right_paired, "%s", line);
            }
        }
        else {
            fprintf(right_single, "%s", headerline);
            right_single_counter++;
            for (int i=0; i<=2; i++) {
                aline = fgets(line, MAXLINELEN, rfp);
                fprintf(right_single, "%s", line);
            }
        }
    }

    /* all that remains is to print the unprinted singles from the left file */

    for (int i = 0; i < opt->tablesize; i++) {
        struct idloc *ptr = ids[i];
        while (ptr != NULL) {
            if (! ptr->printed) {
                fseek(lfp, ptr->pos, SEEK_SET);
                left_single_counter++;
                for (int n=0; n<=3; n++) {
                    aline = fgets(line, MAXLINELEN, lfp);
                    fprintf(left_single, "%s", line);
                }
            }
            ptr = ptr->next;
        }
    }

    fprintf(stderr, "Left paired: %d\t\tRight paired: %d\nLeft single: %d\t\tRight single: %d\n",
            left_paired_counter, right_paired_counter, left_single_counter, right_single_counter);

    fclose(lfp);
    fclose(rfp);

    /*
     * Free up the memory for all the pointers
     */


    for (int i = 0; i < opt->tablesize; i++) {
        struct idloc *ptr = ids[i];
        struct idloc *next;
        while (ptr != NULL) {
            next = ptr->next;
            free(ptr);
            ptr=next;
        }
    }

    free(ids);
    free(line);

    return 0;
}


unsigned hash (char *s) {
    unsigned hashval;

    for (hashval=0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval;
}
