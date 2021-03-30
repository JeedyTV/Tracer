#include "sys_call.h"

char ** get_link(void){

    char **syscall; 
    
    FILE* fichier = NULL;
    
    fichier = fopen("syscall.txt", "r");

    if (fichier != NULL){
        
        int sys_call_number;
        char name_sys_call[25];
        int num_of_sys = 385;

        syscall = malloc(num_of_sys*sizeof(char *));
        if(syscall == NULL) return NULL;
        
        for(int i = 0; i<num_of_sys;i++){
            
            fscanf(fichier, "%d %s",&sys_call_number,name_sys_call);
            syscall[i] = malloc(sizeof(name_sys_call));
            if(syscall[i] == NULL) return NULL;
            strcpy(syscall[i],name_sys_call);
        }
    
        fclose(fichier);

        return syscall;
    }
    
    return NULL;
}

void free_link(char ** tab){

    for(int i=0;i<385;i++) free(tab[i]);
    free(tab);

}

int trace_syscall(char *path){   
    
    pid_t child;
    child = fork();

    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        freopen("/dev/null", "a+", stdout);
        execl(path,get_process_name(path),NULL);
        fclose(stdout);
        return 0;
    }
    else return print_syscall(child);
}

int print_syscall(pid_t child){
        
    int status;
    struct user_regs_struct regs;
    char ** syscall = get_link();
    if(syscall == NULL) return 1;

    while(waitpid(child,&status,0) && ! WIFEXITED(status)) {
        
        if(ptrace(PTRACE_GETREGS, child, NULL, &regs) < 0){
            perror("problem ptrace instruction");
            return 1;
        }
        
        printf("syscall: %s\n",syscall[regs.orig_eax]);
        
        if(ptrace(PTRACE_SYSCALL, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            return 1;
        }
    }

    free_link(syscall);

    return 0;

}

char * get_process_name(char *path){
    
    int index=0;
    
    for(int i=0;path[i] != '\0';i++){
      
      if (  path[i]== '/') index = i;
    
    }
    return path+index+1;

}

Dic * get_labels_dic(char * tracee_name){

    char cmd[DEST_SIZE] ="nm ";
    strcat(cmd, tracee_name);
    strcat(cmd," > nm_tracee_result");
    int cr = system( cmd );
    if ( cr != 0 ){
        fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
        return NULL;
    }

    
    cr = system("wc -l nm_tracee_result > nbre_ligne");
    if ( cr != 0 ){
        fprintf( stderr,"Impossible de lancer la commande : wc\n");
        return NULL;
    }
    
    FILE *fichier = fopen("nbre_ligne","r");
    int nbr_line;
    
    if (fichier != NULL){
        fscanf(fichier, "%d", &nbr_line);

        cr = system("rm nbre_ligne");
        if ( cr != 0 ){
            fprintf( stderr,"Impossible de lancer la commande : rm\n");
            return NULL;
        }

        fclose(fichier);

    }
    
    fichier = fopen("nm_tracee_result","r");
    unsigned long ip;
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
            return NULL;
        }

        return d;
    }

    return NULL;
}

int trace_instruction(char * path){
    
    printf("%s",path); // useless to avoid warning
    return 0;
}