#include "profiler.h"


Dic * get_labels_dic(char * tracee_name,char * regex){

    char cmd[BUFFER_SIZE] ="nm ";
    strcat(cmd, tracee_name);
    strcat(cmd," | grep -E ");
    strcat(cmd,regex);
    strcat(cmd," > nm_tracee_result");
    int cr = system( cmd );
    if ( cr != 0 ){
        exit(1);
    }

    
    cr = system("wc -l nm_tracee_result > nbre_ligne");
    if ( cr != 0 ){
        fprintf( stderr,"Impossible de lancer la commande : wc\n");
        exit(1);
    }
    
    FILE *fichier = fopen("nbre_ligne","r");
    int nbr_line;
    
    if (fichier != NULL){
        fscanf(fichier, "%d", &nbr_line);

        cr = system("rm nbre_ligne");
        if ( cr != 0 ){
            fprintf( stderr,"Impossible de lancer la commande : rm\n");
            exit(1);
        }

        fclose(fichier);

    }
    
    fichier = fopen("nm_tracee_result","r");
    long unsigned ip;
    char trash[30];
    char label[30];
    Dic * d = init_dic();
    
    if (fichier != NULL){

        for(int i=0;i<nbr_line;i++){

            if(fscanf(fichier, "%08lx %s %s", &ip,trash, label)){
                if(!add_el(d,ip,label)) return NULL;

            }  
            else{
                fscanf(fichier, "%s %s",trash, label);
            } 
            
        }
        
        fclose(fichier);

        cr = system("rm nm_tracee_result");
        if ( cr != 0 ){
            fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
            exit(1);
        }

        return d;
    }

    return NULL;
}

Dic * get_labels_dic_o(char * tracee_name){

    char cmd[BUFFER_SIZE] ="objdump -d ";
    strcat(cmd, tracee_name);
    strcat(cmd," | grep -E '([0-9])* <([a-zA-Z_.-])*>:'  > label_tracee_result");
    int cr = system( cmd );
    if ( cr != 0 ){
        fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
        exit(1);
    }
    
    FILE * fichier = fopen("label_tracee_result","r");
    assert(fichier);

    size_t add;
    char label[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    char *re = "%08lx <%[^%s>]>";
    Dic * dic = init_dic();

    while(fgets(line,BUFFER_SIZE, fichier)){

        sscanf(line, re, &add, label);
        if(!add_el(dic,add,label)) return NULL;

    }

    fclose(fichier);

    cr = system("rm label_tracee_result");
    if ( cr != 0 ){
        fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
        exit(1);
    }

    return dic;
    
}

size_t get_start(char * tracee_name){

    char command[BUFFER_SIZE]="objdump -f ";

    strcat(command, tracee_name);

    char * command2 = " | grep 'start' > entry";

    strcat(command, command2);

    int cr = system(command);
    if ( cr != 0 ){
        fprintf(stderr, "Impossible de lancer la commande : %s\n",command);
        exit(1);
    }

    char buffer[BUFFER_SIZE];

    FILE *fichier = fopen("entry","r");

    long unsigned address;

    fscanf(fichier,"%s %s 0x%08lx",buffer,buffer,&address);

    cr = system("rm entry");
    if ( cr != 0 ){
        fprintf(stderr, "Impossible de lancer la commande : rm\n");
        exit(1);
    }

    return address;
}

bool isCall(size_t instruction){
    unsigned int opcode1 = instruction & 0x000000FF;
    return (opcode1 == 0xE8);
}

bool isRet(size_t adresse,unsigned ret_addresse){
    
    return adresse == ret_addresse ;
}

int start_tracer_p(pid_t child,char *programname){
    
    long instruction = 0;
    long return_address = 0;
    struct user_regs_struct regs;
    
    bool just_called = false;
    
    Stack *stack = createStack(5);
    Dic *dic_o = get_labels_dic_o(programname);
    Dic *dic_t = get_labels_dic(programname,"' T | t '");
    Dic *dic_w = get_labels_dic(programname,"' W | w '");
    long start = get_start(programname);
    bool started = false;
    
    int status = 0;

    fun_tree *heap;
    fun_tree *current;
  
    while (waitpid(child,&status,0) && ! WIFEXITED(status)) {
        
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
            
            char * label = get_label(dic_t,regs.eip);
            if(!label) label = get_label(dic_w,regs.eip);
            else if(!label)  label = get_label(dic_o,regs.eip);

            if(!label & just_called){
                just_called = false;
            }

            if (just_called){

                fun_tree *new = new_fun_tree(label, current->depth +1, current,return_address);
                push(stack,new);
                
                if(current->label != label){
                    add(current,new);
                    current = new;
                }
                else{
                    add_rec(current);
                }
                just_called = false;
            }

            if (isCall(instruction)) {
                return_address = regs.eip;
                just_called = true;
            }
            else if(!isEmpty(stack) && isRet(regs.eip,peak(stack)->return_address)){
                pop(stack);

                if(current->recursive && current->recursion_stage){
                    current->recursion_stage--;
                }
                else if(current->prev !=NULL) {
                        current->prev->nb_instructions += current->nb_instructions;
                        current = current->prev;
                        
                    }
            }

        }
        
        if(ptrace(PTRACE_SINGLESTEP, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            exit(1);
        }
        current->nb_instructions++;
        
    }
    
    if(!isEmpty(stack)){
        int nbre = stack->top+1;
        for(int i=0;i<nbre;i++){
                fun_tree * unpoped = pop(stack);
                unpoped->prev->nb_instructions += unpoped->nb_instructions;
        }
    }


    print_tree(heap);
         
    delete_fun_tree(heap);
    free_stack(stack);
    free_dic(dic_t);
    free_dic(dic_w);
    free(dic_o);
    return 0;
}
