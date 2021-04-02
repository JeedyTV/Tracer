#include "sys_call.h"

char ** get_link(void){

    char **syscall; 
    
    FILE* fichier = NULL;
    
    fichier = fopen("syscall.txt", "r");
    assert(fichier);
    
    int sys_call_number;
    char name_sys_call[25];
    int num_of_sys = 385;

    syscall = malloc(num_of_sys*sizeof(char *));
    
    assert(syscall);
    
    for(int i = 0; i<num_of_sys;i++){
        
        fscanf(fichier, "%d %s",&sys_call_number,name_sys_call);
        syscall[i] = malloc(sizeof(name_sys_call));
        
        assert(syscall[i]);
        strcpy(syscall[i],name_sys_call);
    }

    fclose(fichier);

    return syscall;
}

void free_link(char ** tab){

    for(int i=0;i<385;i++) {
        free(tab[i]);
       
    }
    free(tab);
   

}

int start_tracer_s(pid_t child){
        
    int status;
    struct user_regs_struct regs;
    char ** syscall = get_link();
    if(syscall == NULL) return 1;

    while(waitpid(child,&status,0) && ! WIFEXITED(status)) {
        
        if(ptrace(PTRACE_GETREGS, child, NULL, &regs) < 0){
            perror("problem ptrace instruction");
            exit(1);
        }
        
        printf("syscall: %s\n",syscall[regs.orig_eax]);
        
        if(ptrace(PTRACE_SYSCALL, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            exit(1);
        }
    }

    free_link(syscall);

    return 0;

}


