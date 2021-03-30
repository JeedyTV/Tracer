/* This file represents a sample tracee program
 * that must be compiled as follow:
 *      gcc --static -g -o tracee tracee.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "syscall.h"
#include <stdbool.h>

int main(int argc, char *argv[])  
{ 
    if (argc != 3){
        // on sommace
        printf("./tracer <mode> <prog>\n where <mode> is either -p to profile the program (profiler mode) or \n-s to get the list of system calls used by the program (syscall mode)\n");
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
            printf("./tracer <mode> <prog>\n where <mode> is either -p to profile the program (profiler mode) or \n-s to get the list of system calls used by the program (syscall mode)\n");
            return 1;
            break;
        }
        
    } 
}

