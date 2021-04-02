#ifndef SYSCALL
#define SYSCALL

#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <syscall.h>
#include <sys/reg.h>
#include <sys/ptrace.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <assert.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdbool.h>

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

/**
 * wait the child
 * @param child process id of the tracee
 * @return 0 if no problem.
 */
int wait_syscall(pid_t child);

#endif
