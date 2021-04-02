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
    if (argc != 3){
        printf("2 argmuents are required\n");
        printf("./tracer <mode> <prog>\n");
        exit(1);
    }
    else{
        char * path;
        path = argv[2];

        switch (argv[1][1])
        {
        case 's':
            return start_tracer(path,'s');
            break;
        case 'p':
            return start_tracer(path,'p');
            break;
        default:
            printf("should use as follows :\n");
            printf("./tracer <mode> <prog>\n");
            printf("where <mode> is either -p or -s\n");
            exit(1);
            break;
        } 
    } 
}

