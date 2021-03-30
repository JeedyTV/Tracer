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
    }
    else{

        char * path;

        path = argv[2];

        //switch
        //change return

        if (argv[1][1] == 's'){
            trace_syscall(path);
            return 0;

        }
        if (argv[1][1] == 'p'){
            //call qq
            return 0;
        }

    }
    printf("./tracer <mode> <prog>\n where <mode> is either -p to profile the program (profiler mode) or \n-s to get the list of system calls used by the program (syscall mode)\n");
    return 1; 
}

