/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2023.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 7-1 */

//#define _BSD_SOURCE
//#include "tlpi_hdr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "HMM.h"

#define MAX_ALLOCS 1000000

#define getInt(x, ...) atoi(x)

extern char end, edata, etext;

/* Enable if you want to replace libc malloc/free */
#if 0
void * malloc(size_t size)
{
    return NULL;
}

void free(void *ptr)
{
    (void) ptr;
}

void *calloc(size_t nmemb, size_t size)
{
	return NULL;
}

void *realloc(void *ptr, size_t size)
{
	return NULL;
}
#endif

//Error code is used for only testing purposes
extern char error_code;
extern char blocks_count;
int j;

int main( int argc, char *argv[])
{
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs;//, j;
    printf("etext = %p, edata=%p, end=%p, initial program break=%p\n", &etext, &edata, &end, (char*)sbrk(0));
    //getchar();

     char* initial_prog_brk = (char*)sbrk(0);

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("%s num-allocs block-size [step [min [max]]]\n", argv[0]);
        exit(1);
    }

    numAllocs = getInt(argv[1], GN_GT_0, "num-allocs");
    if (numAllocs > MAX_ALLOCS) {
        printf("num-allocs > %d\n", MAX_ALLOCS);
        exit(1);
    }

    blockSize = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "block-size");

    freeStep = (argc > 3) ? getInt(argv[3], GN_GT_0, "step") : 1;
    freeMin =  (argc > 4) ? getInt(argv[4], GN_GT_0, "min") : 1;
    freeMax =  (argc > 5) ? getInt(argv[5], GN_GT_0, "max") : numAllocs;

    if (freeMax > numAllocs) {
        printf("free-max > num-allocs\n");
        exit(1);
    }

    printf("Initial program break:          %10p\n", sbrk(0));

    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
    for (j = 0; j < numAllocs; j++) {
        printf("----------------Iteration %d started --------------\n",(j+1));
        ptr[j] = HmmAlloc(blockSize);
       printf("----------------Iteration %d ended --------------\n",(j+1));
        if (ptr[j] == NULL) 
        {
            printf("error_code = %d\n",error_code);
            printf("Hmm_malloc returned null\n");
            exit(1);
        }
    }

    printf("initial Program break is :           %10p\n",initial_prog_brk);
    printf("Program break is now:           %10p\n", sbrk(0));
    printf("No of allocated pages = %d\n",blocks_count);

    printf("Freeing blocks from %d to %d in steps of %d\n",
                freeMin, freeMax, freeStep);
    for (j = freeMin - 1; j < freeMax; j += freeStep)
        HmmFree(ptr[j]);

    printf("After free(), program break is: %10p\n", sbrk(0));
    while(1);

    exit(EXIT_SUCCESS);
}
