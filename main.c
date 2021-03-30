#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h> 
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/ptrace.h>
#include <sys/reg.h> /* EAX, ORIG_EAX */
#include <sys/user.h>
#include "sys_call.h"


int wait_(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    if(WIFSTOPPED(status)) return 0;
    else if (WIFEXITED(status)) return 1;
    else {
        printf("%d status should be STOP or EXIT\n", pid);
        return 1;
    }
}

bool isCall(unsigned long instruction){
    unsigned int opcode1 = instruction & 0x000000FF;
    return (opcode1 == 0x9A || opcode1 == 0xE8 || opcode1 == 0xFF);
}
bool isRet(unsigned long instruction){
    unsigned int opcode1 = instruction &0x000000FF;
    return (opcode1 >= 0xC2 && opcode1 <= 0xC3) || (opcode1 >= 0xCA && opcode1 <= 0xCB);
}
int start_tracer(pid_t child,char *programname){
    unsigned long instruction, ip;
    Dic *d = get_labels_dic(programname);
    // if want a label = get_label(d,ad_label);
    
    while (wait_(child) < 1) {

        // FUN function qui permet de lier etiquetee a l'ip

        // ip - inst 

        // construitre l'arbe 

        // FUN cree une 2 focntion qui en prent opcode true si c call et true si c ret et false sinon .

        // count ins***
	
        ip = ptrace(PTRACE_PEEKUSER, child, 4 * EIP, NULL);
	if(ip < 0){
            perror("problem ptrace ip");
            return 1;
        }
	instruction = ptrace(PTRACE_PEEKTEXT, child, ip, NULL);
	if(instruction < 0){
            perror("problem ptrace instruction");
            return 1;
        }
        
        printf("--%08lx--,--%08lx--\tcall:%d,ret:%d\n",instruction,ip,isCall(instruction),isRet(instruction));
        if(ptrace(PTRACE_SINGLESTEP, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            return 1;
        }

	
    }
    return 0;
}


int fork_and_trace(char *programname) {
    
    pid_t child_pid = fork();
    if (child_pid == 0){

        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
            perror("problem for the ptrace");
            return 1;
        }
        if (execl(programname, programname,NULL) < 0){
            perror("can't execute the tracee");
            return 1;
        }else return 0;
    }else if (child_pid > 0) return start_tracer(child_pid,programname);
    else {
        perror("problem for fork");
        return 1;
    }
}


int main(int argc, char** argv)
{

    /* Run and trace program. */
    return fork_and_trace(argv[1]);

    return 0;
}
