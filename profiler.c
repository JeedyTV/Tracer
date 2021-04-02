#include "profiler.h"


Dic * get_labels_dic(char * tracee_name,char * regex){

    /* writing nm command */
    char cmd[BUFFER_SIZE] ="nm ";
    strcat(cmd, tracee_name);
    strcat(cmd," | grep -E ");
    strcat(cmd,regex);
    strcat(cmd," > nm_tracee_result");  /* put nm in nm_tracee_result */
    /* excecute nm */
    int cr = system( cmd );
    if ( cr != 0 ){
        exit(1);
    }

    
    cr = system("wc -l nm_tracee_result > nbre_ligne"); /* put number of functions in nbre_ligne */
    if ( cr != 0 ){ /* Unexpected behaviour */
        fprintf( stderr,"Impossible de lancer la commande : wc\n");
        exit(1);
    }
    
    /* Open number of functions */
    FILE *fichier = fopen("nbre_ligne","r");
    int nbr_line;   /* number of functions */
    
    if (fichier != NULL){
        fscanf(fichier, "%d", &nbr_line);

        cr = system("rm nbre_ligne");
        if ( cr != 0 ){
            fprintf( stderr,"Impossible de lancer la commande : rm\n");
            exit(1);
        }

        fclose(fichier);

    }
    
    /* Open function names */
    fichier = fopen("nm_tracee_result","r");
    long unsigned ip;   /* virtual address of symbols */
    char trash[30];
    char label[30];     /* symbols = function names */
    Dic * d = init_dic();   /* stores (ip,label) in a dictionnary */
    
    if (fichier != NULL){
        for(int i=0;i<nbr_line;i++){
            if(fscanf(fichier, "%08lx %s %s", &ip,trash, label)){
                if(!add_el(d,ip,label)) 
                    return NULL;
            }  
            else{
                fscanf(fichier, "%s %s",trash, label);
            }          
        }
        
        fclose(fichier);

        cr = system("rm nm_tracee_result");
        if ( cr != 0 ){ /* Unexpected behaviour */
            fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
            exit(1);
        }

        return d;
    }

    return NULL;
}

Dic * get_labels_dic_o(char * tracee_name){

    /* writing objdump command */
    char cmd[BUFFER_SIZE] ="objdump -d ";
    strcat(cmd, tracee_name);
    /* put objdump addresss and function names in label_tracee_result */
    strcat(cmd," | grep -E '([0-9])* <([a-zA-Z_.-])*>:'  > label_tracee_result");
    /* excecute objdump */
    int cr = system( cmd );
    if ( cr != 0 ){ /* Unexpected behaviour */
        fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
        exit(1);
    }
    
    /* Open results */
    FILE * fichier = fopen("label_tracee_result","r");
    assert(fichier);

    size_t add;     /* address of functions */
    char label[BUFFER_SIZE];    /* function names */
    char line[BUFFER_SIZE];
    char *re = "%08lx <%[^%s>]>";
    Dic * dic = init_dic();     /* stores (add,label) in a dictionnary */

    while(fgets(line,BUFFER_SIZE, fichier)){
        sscanf(line, re, &add, label);
        if(!add_el(dic,add,label)) return NULL;

    }

    fclose(fichier);

    cr = system("rm label_tracee_result");
    if ( cr != 0 ){ /* Unexpected behaviour */
        fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
        exit(1);
    }

    return dic;    
}

size_t get_start(char * tracee_name){

    /* writing command */
    char command[BUFFER_SIZE]="objdump -f ";
    strcat(command, tracee_name);

    char * command2 = " | grep 'start' > entry";    /* start the tracee process */
    strcat(command, command2);

    /* excute command */
    int cr = system(command);
    if ( cr != 0 ){ /* Unexpected behaviour */
        fprintf(stderr, "Impossible de lancer la commande : %s\n",command);
        exit(1);
    }

    char buffer[BUFFER_SIZE];

    FILE *fichier = fopen("entry","r");

    long unsigned address;  /* entry point address */

    fscanf(fichier,"%s %s 0x%08lx",buffer,buffer,&address);

    cr = system("rm entry");
    if ( cr != 0 ){ /* Unexpected behaviour */
        fprintf(stderr, "Impossible de lancer la commande : rm\n");
        exit(1);
    }

    return address;
}

bool isCall(size_t instruction){
    unsigned int opcode1 = instruction & 0x000000FF;
    /* opcodes used to detect a call */
    return (opcode1 == 0x9A || opcode1 == 0xE8 || opcode1 == 0xFF);
}

bool isRet(size_t address,unsigned ret_address){
    /* compare address with expected return address */
    return address == ret_address ;
}

int start_tracer_p(pid_t child,char *programname){
    
    long instruction = 0;      /* the current instruction */
    long return_address = 0;   /* address of each instruction */ 
    bool just_called = false;  /* true if the previous intruction was a call */
    struct user_regs_struct regs;   /* the current address data */
    
    Stack *stack = createStack(5);  /* used for return functions */

    /* Dictionnaries of address and associated name of all instructions of the process */
    Dic *dic_o = get_labels_dic_o(programname);
    Dic *dic_t = get_labels_dic(programname,"' T | t '");
    Dic *dic_w = get_labels_dic(programname,"' W | w '");

    /* starting the process */
    long start = get_start(programname);
    bool started = false;   /* true when the current instruction has passed the entry point */
    
    int status = 0;         /* for the wait */

    fun_tree *heap;
    fun_tree *current;
  
    /* Loop until the child process finishes
        Each iteration advances the process by one instruction */
    while (waitpid(child,&status,0) && ! WIFEXITED(status)) {
        
        /* Copy the tracee's general-purpose registers, to the regs in the tracer */
        if(ptrace(PTRACE_GETREGS, child, NULL, &regs) < 0){
            perror("problem regs");
            exit(1);
        }

        instruction = ptrace(PTRACE_PEEKTEXT, child, regs.eip, NULL);
        
        return_address = ptrace(PTRACE_PEEKTEXT, child, regs.esp, NULL);
        
        if(start == regs.eip) {
            started = true;
            heap = new_fun_tree(get_label(dic_t,regs.eip), 0, NULL,return_address);
            current = heap;
        }
        
        if(started){
            
            /* search for the instruction name */
            char * label = get_label(dic_t,regs.eip);
            if(!label) label = get_label(dic_w,regs.eip);
            else if(!label)  label = get_label(dic_o,regs.eip);

            /* Unexpected: instruction which name is not in the dictionnary */
            if(!label & just_called){
                just_called = false;
            }

            /* if the previous instruction was a call, */
            if (just_called){

                /* creating a new call function tree */
                fun_tree *new = new_fun_tree(label, current->depth +1, current,return_address);
                push(stack,new);    /* push it in the stack to pop it when returns */
                
                if(current->label != label){    /* NOT a recursion */
                    add(current,new);
                    current = new;
                }
                else{                           /* if a recursion */
                    add_rec(current);
                }
                just_called = false;
            }

            /* if the instruction is a call */
            if (isCall(instruction)) {
                return_address = regs.eip;
                just_called = true;
            }

            /* if the instruction is a return */
            else if(!isEmpty(stack) && isRet(regs.eip,peak(stack)->return_address)){
                pop(stack);

                /* if return inside a recursion cascade */
                if(current->recursive && current->recursion_stage){
                    current->recursion_stage--;
                }
                /* if not a return from a recursion call */
                else if(current->prev !=NULL) {
                    /* increment the caller function with the instructions of the called function */
                    current->prev->nb_instructions += current->nb_instructions;
                    current = current->prev;                        
                }
            }
        }
        
        /* Trace instructions step by step */
        if(ptrace(PTRACE_SINGLESTEP, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            exit(1);
        }

        /* increment instructions of the function by 1 */
        current->nb_instructions++;        
    }
    
    if(!isEmpty(stack)){
        int nbre = stack->top+1;
        for(int i=0;i<nbre;i++){
                fun_tree * unpoped = pop(stack);
                unpoped->prev->nb_instructions += unpoped->nb_instructions;
        }
    }

    /* display the full tree of called functions */
    print_tree(heap);
         
    /* Clear memory */
    delete_fun_tree(heap);
    free_stack(stack);
    free_dic(dic_t);
    free_dic(dic_w);
    free(dic_o);
    return 0;
}
