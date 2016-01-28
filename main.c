static char const cvsid[] = "$Id: main.c,v 2.1 2005/06/14 22:16:50 jls Exp $";

/*
 * Copyright 2005 SRC Computers, Inc.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, Inc.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, Inc.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <libmap.h>
#include <stdlib.h>

#define SZ 65536

void subr (int64_t In[], int64_t Out[], int num, int64_t *dma_time, int64_t *compute_time, int mapnum);

int main (int argc, char *argv[]) {
    FILE *res_map, *res_cpu;
    int i, num;
    int64_t *A, *B;
    int64_t dma_tm, compute_tm;
    int mapnum = 0;

    if ((res_map = fopen ("res_map", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_map'\n");
        exit (1);
        }

    if ((res_cpu = fopen ("res_cpu", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_cpu'\n");
        exit (1);
        }

    if (argc < 2) {
	fprintf (stderr, "need number of elements (up to %d) as arg\n", SZ);
	exit (1);
	}

    if (sscanf (argv[1], "%d", &num) < 1) {
	fprintf (stderr, "need number of elements (up to %d) as arg\n", SZ);
	exit (1);
	}

    if (num > SZ) {
	fprintf (stderr, "need number of elements (up to %d) as arg\n", SZ);
	exit (1);
	}

    A = (int64_t*) malloc (SZ * sizeof (int64_t));
    B = (int64_t*) malloc (SZ * sizeof (int64_t));

    srandom (99);

    for (i=0; i<SZ; i++) {
        A[i] = random () & 0xffff;
	}

    map_allocate (1);

    // call the MAP routine
    subr (A, B, num, &dma_tm, &compute_tm, mapnum);

    printf ("DMA to MAP:     %10lld clocks\n", dma_tm);
    printf ("compute on MAP: %10lld clocks\n", compute_tm);

    for (i=0; i<num; i++) {
        fprintf (res_map, "%lld\n", B[i]);
        fprintf (res_cpu, "%lld\n", (A[i]+42)*17);
	}

    map_free (1);

    exit(0);
    }
