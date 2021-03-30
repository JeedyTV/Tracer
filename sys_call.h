#ifndef SYSCALL
#define SYSCALL

#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <syscall.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "dictionnary.h"

#define DEST_SIZE 50

char ** get_link(void);
void free_link(char ** tab);
int trace_syscall(char *path);
char * get_process_name(char *path);
Dic * get_labels_dic(char * tracee_name);
int trace_instruction(char * path);
int print_syscall(pid_t child);

#endif
