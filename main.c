#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <stdbool.h>
//#include "fun_tree.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/ptrace.h>
#include <sys/reg.h> /* EAX, ORIG_EAX */
#include <sys/user.h>
#include "sys_call.h"
#include "stack.h"

unsigned long get_start(char * tracee_name){

    char command[DEST_SIZE]="readelf --segments ";

    strcat(command, tracee_name);

    char * command2 = " | grep 'Entry point' > entry";

    strcat(command, command2);

    int cr = system(command);
    if ( cr != 0 ){
        fprintf(stderr, "Impossible de lancer la commande : readelf\n");
        return 0;
    }

    char trash[DEST_SIZE];

    FILE *fichier = fopen("entry","r");

    unsigned long address;

    fscanf(fichier,"%s %s 0x%lx",trash,trash,&address);

    cr = system("rm entry");
    if ( cr != 0 ){
        fprintf(stderr, "Impossible de lancer la commande : rm\n");
        return 0;
    }

    return address;
}

bool isCall(unsigned long instruction){
    unsigned long opcode = instruction & 0x000000FF;
    return (opcode == 0xE8);
}

bool is_ret(unsigned long instruction){
    unsigned long opcode = instruction & 0x000000FF;
    return opcode == 0xC2 || opcode == 0xC3 || opcode == 0xCA ||
           opcode == 0xCB || opcode == 0xCF;

}

bool isRet(unsigned long adresse,unsigned ret_addresse){
    
    return adresse == ret_addresse ;
}

int start_tracer(pid_t child,char *programname){
    
    unsigned long instruction = 0;
    unsigned long ir = 0;
    struct user_regs_struct regs;
    bool called = false;
    bool ret = false;
    Stack *stack = createStack(2);
    Dic *d = get_labels_dic(programname);
    unsigned long start = get_start(programname);
    bool started = false;

    int count1 = 0;
    int count2 = 0;
    int count3 = 0;  

    int status = 0;

    int i = 0;
    
    while (waitpid(child,&status,0) && ! WIFEXITED(status)) {

        // TODO:FUN function qui permet de lier etiquetee a l'ip

        // ip - inst 

        // TODO: construitre l'arbe 

        // FAIT FUN cree une 2 focntion qui en prent opcode true si c call et true si c ret et false sinon .

        // count ins***

        if(ptrace(PTRACE_GETREGS, child, NULL, &regs) < 0){
            perror("problem regs");
            return 1;
        }
	
        instruction = ptrace(PTRACE_PEEKTEXT, child, regs.eip, NULL);
        if(instruction < 0){
                perror("problem ptrace instruction");
                return 1;
        }

        ir = ptrace(PTRACE_PEEKTEXT, child, regs.esp, NULL);
        if(ir < 0){
                perror("problem ptrace instruction");
                return 1;
        }

        if(start == regs.eip) {
            started = true;
            printf("THe entry point");
        }

        if(started){

            i++;

            if (called) {
                push(stack,ir);
                printf("called: %s\n",get_label(d,regs.eip));
                called = false;
            }
            
            if(isCall(instruction)) {
                
                called = true;
                count1++;
            }

            
            if(isRet(regs.eip,peak(stack))){
                printf("ret\n");
                pop(stack);
                count3++;
            }

        }

        if(ptrace(PTRACE_SINGLESTEP, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            return 1;
        }

    }

    free_stack(stack);
    printf("call : %d ret: %d\n",count1,count3);

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
