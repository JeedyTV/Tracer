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

/**
 * Stores in a dictionnary the name of every function called by a process
 * and their linked address using nm command.
 * @param tracee_name the name of the process
 * @param regex nm options
 * @return a dictionnary
 */
Dic * get_labels_dic(char * tracee_name,char * regex);

/**
 * Stores in a dictionnary the name of every function called by a process
 * and their linked address using objdump command.
 * @param tracee_name the name of the process
 * @return a dictionnary
 */
Dic * get_labels_dic_o(char * tracee_name);

/**
 * Detectes a call instruction.
 * @param intruction opcode of an instruction
 * @return true if is a call function, false otherwise.
 */
bool isCall(size_t instruction);

/**
 * Detectes a return instruction.
 * @param address address of an instruction
 * @param ret_address address the expected return
 * @return true if is a return function, false otherwise.
 */
bool isRet(size_t address,unsigned ret_address);

/**
 * Traces a process instruction by instruction
 * and print all its functions according to the protocol.
 * @param child process id of the traced process
 * @param programname name of the process
 * @return 0 if no problem.
 */
int start_tracer_p(pid_t child,char *programname);

#endif