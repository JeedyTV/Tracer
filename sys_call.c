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
        

        for(int i = 0; i<num_of_sys;i++){
            
            fscanf(fichier, "%d %s",&sys_call_number,name_sys_call);
            
            int size=0;

            syscall[i] = malloc(sizeof(name_sys_call));

            strcpy(syscall[i],name_sys_call);

        }
        
        fclose(fichier);
        
    }

    return syscall;

}

void free_link(char ** tab){

    for(int i=0;i<385;i++) free(tab[i]);
    free(tab);

}

void trace_syscall(char *path){   
    
    pid_t child;
    
    struct user_regs_struct regs;
    
    child = fork();
    
    if(child == 0) {
        
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        freopen("/dev/null", "a+", stdout);
        execl(path,get_process_name(path),NULL);
        fclose(stdout);

    }
    else{
        
        int status;

        char ** syscall = get_link();

        while(waitpid(child,&status,0) && ! WIFEXITED(status)) {
          
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            
            printf("syscall: %s\n",syscall[regs.orig_eax]);
            
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }

        free_link(syscall);
    }

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
    
    if(d == NULL) return NULL;
    
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
    }

    cr = system("rm nm_tracee_result");
    if ( cr != 0 ){
        fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
        return NULL;
    }

    return d;
}
