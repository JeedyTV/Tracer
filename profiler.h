#ifndef profiler
#define profiler

#include "dictionnary.h"
#include <assert.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include "fun_tree.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/reg.h> 
#include <sys/user.h>
#include "sys_call.h"
#include "stack.h"


#define BUFFER_SIZE 250

Dic * get_labels_dic(char * tracee_name,char * regex);
Dic * get_labels_dic_o(char * tracee_name);
size_t get_start(char * tracee_name);
bool isCall(size_t instruction);
bool isRet(size_t adresse,unsigned ret_addresse);
int start_tracer_p(pid_t child,char *programname);

#endif