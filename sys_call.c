#include "sys_call.h"

char ** get_link(void){

    char **syscall; 
    
    FILE* fichier = NULL;
    
    fichier = fopen("syscall.txt", "r");
    assert(fichier);
    
    int sys_call_number;    /* each syscall number */
    char name_sys_call[25]; /* each syscall name */
    int num_of_sys = 385;   

    /* allocate a table */
    syscall = malloc(num_of_sys*sizeof(char *));
    
    assert(syscall);
    
    for(int i = 0; i<num_of_sys;i++){
        
        /* Retrieve each syscall name and associated number */
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
        
    int status;     /* for the wait */
    struct user_regs_struct regs;   /* the current address data */
    char ** syscall = get_link();   /* contains syscal names */
    if(syscall == NULL) return 1;

    /* Loop until the child process finishes 
        Each iteration advances the process to the next
        entry to (or exit from) a syscall */
    while(waitpid(child,&status,0) && ! WIFEXITED(status)) {
        
        /* Copy the tracee's general-purpose registers, to the regs in the tracer */
        if(ptrace(PTRACE_GETREGS, child, NULL, &regs) < 0){
            perror("problem ptrace instruction");
            exit(1);
        }
        
        /* print according to the protocol */
        printf("syscall: %s\n",syscall[regs.orig_eax]);
        
        /* Inspect the return value of the system call */
        if(ptrace(PTRACE_SYSCALL, child, NULL, NULL) < 0){
            perror("problem ptrace instruction");
            exit(1);
        }
    }

    free_link(syscall);

    return 0;
}


