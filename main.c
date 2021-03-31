#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <stdbool.h>
#include "fun_tree.h"
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
    unsigned int opcode1 = instruction & 0x000000FF;
    return (opcode1 == 0x9A || opcode1 == 0xE8 || opcode1 == 0xFF);
}

bool isRet(unsigned long adresse,unsigned ret_addresse){
    
    return adresse == ret_addresse ;
}

int start_tracer(pid_t child,char *programname){
    unsigned long instruction = 0;
    unsigned long return_address = 0;
    struct user_regs_struct regs;
    fun_tree *heap = NULL;
   
    size_t nb_instr = 0;
    size_t nb_calls = 0;
    size_t nb_returns = 0;
    
    bool just_called = false;
    bool just_returned = false;
    Stack *stack = createStack(2);
    Dic *d = get_labels_dic(programname);
    unsigned long start = get_start(programname);
    bool started = false;
    fun_tree *current = heap;
    
    int status = 0;
  
    while (waitpid(child,&status,0) && ! WIFEXITED(status)) {
        if(ptrace(PTRACE_GETREGS, child, NULL, &regs) < 0){
            perror("problem regs");
            return 1;
        }

        instruction = ptrace(PTRACE_PEEKTEXT, child, regs.eip, NULL);
        if(instruction < 0){
                perror("problem ptrace instruction");
                return 1;
        }
        return_address = ptrace(PTRACE_PEEKTEXT, child, regs.esp, NULL);
        if(return_address < 0){
                perror("problem ptrace instruction");
                return 1;
        }

        if(ptrace(PTRACE_SINGLESTEP, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            return 1;
        }
        

        if(start == regs.eip) {
            started = true;
            printf("THe entry point\n");
        }


        if(started){
            nb_instr++;

            if (just_called){
                push(stack,return_address);
                if (!heap){
                    printf("premier call:\n");
                    heap = new_fun_tree("le nom de l'instru", 0, NULL);
                    current = heap;
                    nb_instr = 0;
                    just_called = false;
                    ++nb_calls;
                    continue;
                }

                current->nb_instructions += nb_instr;
                nb_instr = 0;
                if(current->label != "le nom de l'instru"){
                    printf("called: %s\n",get_label(d,regs.eip));
                    current->next = new_fun_tree("le nom de la nouvelle instru appelée", current->depth +1, current);
                    current = current->next;
                    ++nb_calls;
                }
                else{
                    printf("call de récursion:\n");
                    current->recursive = true;
                    current->nb_rec_calls++;
                    current->recursion_stage++;
                }
                just_called = false;
            }
            else if(just_returned && heap){
                printf("fin de la fonction\n");
                if(current->recursive && current->recursion_stage){
                    printf("et c'est une récursion\n");
                    current->nb_instructions += nb_instr;
                    nb_instr =0;
                    current->recursion_stage--;
                }
                else{
                    printf("et ce n'était pas une récursion\n");
                    current->nb_instructions += nb_instr;
                    nb_instr = 0;
                    ++nb_returns;
                    if(current->prev){
                        current->prev->subtree = current;
                        current->prev->next = NULL;
                        current= current->prev;
                    }
                    else{
                        printf("dernière function");
                        print_tree(current);
                        delete_fun_tree(current);
                        break;
                    }
                }
                just_returned = false;
            }
            if (isCall(instruction)) {
                printf("called: %s\n",get_label(d,regs.eip));
                just_called = true;
            }
            else if(isRet(regs.eip,peak(stack))){
                printf("Y a un return\n");
                pop(stack);
                just_returned = true;
            }
            //printf("--%08lx--,--%08lx--\tcall:%d,ret:%d\n",instruction,ip,nb_calls,nb_returns);
        }
        // do
        // {
        //     printf("--%08lx--,--%08lx--\tcall:%d,ret:%d\n",instruction,ip,isCall(instruction),isRet(instruction));

        //     son = current;
        //     current = son->next;
        //     son->nb_instructions += instruction;
        //     instruction = 0;

        //     father = current;
        //     if(father) {
        //         if (!father->next)
        //             father->next = new_fun_tree(NULL,0,NULL);
        //         father->next = son;
        //         father->nb_instructions += son->nb_instructions;
        //     }
        // } while (father);
        
    }

    // current = heap;
    // while(current != NULL){
    //     print_tree(current);
    //     fun_tree *prec = current;
    //     current = current->next;
    //     delete_fun_tree(prec);
    // }
    free_stack(stack);
    free_dic(d);
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
