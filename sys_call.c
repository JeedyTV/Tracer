#include <stdio.h>
#include <stdlib.h>
#include "sys_call.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <string.h>
#include <syscall.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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
            
            while(name_sys_call[size] != '\0') size++;

            syscall[i] = malloc(sizeof(char)*size);

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