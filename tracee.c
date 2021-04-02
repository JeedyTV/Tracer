/* This file represents a sample tracee program
 * that must be compiled as follow:
 *      gcc --static -g -o tracee tracee.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "tracer.h"
#include <stdbool.h>

int main(int argc, char *argv[])  
{ 
    if (argc != 3){ /* Command protocol NOT respected */
        printf("2 argmuents are required\n");
        printf("./tracer <mode> <prog>\n");
        exit(1);
    }
    else{           /* Command protocol respected */
        char * path;
        path = argv[2];

        switch (argv[1][1])
        {
        case 's':   /* The syscall mode */
            return start_tracer(path,'s');
            break;
        case 'p':   /* The profiler mode */
            return start_tracer(path,'p');
            break;
        default:    /* Unexpected */
            printf("should use as follows :\n");
            printf("./tracer <mode> <prog>\n");
            printf("where <mode> is either -p or -s\n");
            exit(1);
            break;
        } 
    } 
}

