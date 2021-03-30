/* This file represents a sample tracee program
 * that must be compiled as follow:
 *      gcc --static -g -o tracee tracee.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "sys_call.h"
#include <stdbool.h>

int main(int argc, char *argv[])  
{ 
    if (argc != 3){
        printf("2 argmuents are required\n");
        printf("./tracer <mode> <prog>\n");
        return 1;
    }
    else{
        char * path;
        path = argv[2];

        switch (argv[1][1])
        {
        case 's':
            return trace_syscall(path);
            break;
        case 'p':
            return trace_instruction(path);
            break;
        default:
            printf("should use as follows :\n");
            printf("./tracer <mode> <prog>\n");
            printf("where <mode> is either -p or -s\n");
            return 1;
            break;
        } 
    } 
}

