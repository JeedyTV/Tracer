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

/**
 * Creates a table that contains all sycalls from "syscall.txt"
 * @return table of syscall names
 */
char ** get_link(void);

/**
 * Free a table of char
 * @param tab
 */
void free_link(char ** tab);

/**
 * Traces a process syscall by syscall
 * and print all of them by name according to the protocol.
 * @param child process id of the tracee
 * @return 0 if no problem.
 */
int start_tracer_s(pid_t child);

#endif
