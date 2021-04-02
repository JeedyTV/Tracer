#ifndef TRACER
#define TRACER

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <stdio.h>
#include "sys_call.h"
#include "profiler.h"

int start_tracer(char *path,char mode);
char * get_tracee_name(char *path);

#endif