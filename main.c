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

//just laisser e8 car on veut reln call
bool isCall(unsigned long instruction){
    unsigned int opcode = instruction & 0x000000FF;
    return (opcode == 0xE8);
}
bool isRet(unsigned long adresse,unsigned ret_addresse){
    
    return adresse == ret_addresse ;
}

int start_tracer(pid_t child,char *programname){
    unsigned long instruction = 0;
    unsigned long ir = 0;
    struct user_regs_struct regs;
    bool called = false;
    Stack *stack = createStack(2);
    //fun_tree* heap = new_fun_tree("", 0, NULL);

    //fun_tree* current = heap;
    Dic *d = get_labels_dic(programname);
    // if want a label = get_label(d,ad_label);

    int count1 = 0;
    int count2 = 0;  

    int status = 0;
    
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

        if (called) {
            push(stack,ir);
            called = false;
        }
        if(isCall(instruction)) {
            called = true;
            count1++;
        }

        if(isRet(regs.eip,peak(stack))) {
            pop(stack);
            count2++;
        }
        
        //printf("%08lx %08lx ret: %08lx\n",instruction,regs.eip,ir);
        //printf("--%08lx--,--%lu--\tcall:%d,ret:%d\n",instruction,ip,isCall(instruction),isRet(instruction));
        //For a new function
        //if (!current->label)
            //current->label = get_label(instriction);
        
        // If calling a function
        //if(isCall(instruction)){
            // If recusion
            //current->recusion = true;
            //current->nb_recursions++;
            // Else

            current->next = new_fun_tree(get_label(d,ip), 0, NULL);
            current->next->parent = current;
            current->next->depth = current->depth + 1;
            current = current->next;
        }
        // If retunring from a function
        else if(isRet(instruction)){
            current->parent += ++current->nb_instructions;
            current = current->parent;
        }

        // If continuing in the same function
        else current->nb_instructions++;
            
        //}
	// If retunring from a function

	// If continuing in the same function
	    //current->nb_instructions++;

        if(ptrace(PTRACE_SINGLESTEP, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            return 1;
        }
    }

    current = heap;
    while(current){
        print_tree(current);
        current = current->next;
    }


    free_stack(stack);
    printf("call : %d ret: %d\n",count1,count2);

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
    //fun_tree* test_tree = new_fun_tree("instru", 1, NULL);
    //delete_fun_tree(test_tree);
    /* Run and trace program. */
    return fork_and_trace(argv[1]);

    return 0;
}
