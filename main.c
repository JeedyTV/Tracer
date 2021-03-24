#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <string.h>
#include <syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hello.h"

#define DEST_SIZE 40
#define STRING_SIZE 25

int main(){   
    
    pid_t child;
    
    struct user_regs_struct regs;
    
    child = fork();
    
    if(child == 0) {
        
    
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        
        char src[] = "/dev/null";
	    char dest[DEST_SIZE] = "/bin/ls > ";
	    strcat(dest, src);
        
        execl("/bin/sh", "sh", "-c", dest,NULL);
    }
    else {
        int status;

        char ** syscall = get_link();

        while(waitpid(child,&status,0) && ! WIFEXITED(status)) {
          
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            
            printf("%ld %s\n",regs.orig_eax,syscall[regs.orig_eax]);
            
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }

        free_link(syscall);
            
   }
   return 0;
}
