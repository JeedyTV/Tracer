#ifndef SYSCALL
#define SYSCALL

#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <assert.h>
#include <sys/user.h>


char ** get_link(void);
void free_link(char ** tab);
int start_tracer_s(pid_t child);

#endif
