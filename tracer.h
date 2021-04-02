#ifndef TRACER
#define TRACER

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <stdio.h>
#include "sys_call.h"
#include "profiler.h"

/**
 * Creates a child process traced by its parent,
 * while the parent process starts tracing its child 
 * with behaviour depending on the mode parameter.
 * @param path a string which represents the process which will be traced.
 * @param mode a string which represents the mode ('s' or 'p').
 * @return 0 if ok. Exits program otherwise.
 */
int start_tracer(char *path,char mode);

/**
 * Retrieves the name of the name of the process which will be traced.
 * @param path the path of a process.
 * @return the name of the process.
 */
char * get_tracee_name(char *path);

#endif