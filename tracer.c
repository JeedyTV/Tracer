#include "tracer.h"

int start_tracer(char *path,char mode) {

    char * tracee_name = get_tracee_name(path);

    /* fork a child process */
    pid_t child_pid = fork();
    
    if (child_pid == 0){    /* Child process */

        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
            perror("problem for the ptrace");
            exit(1);
        }
        
        freopen("/dev/null", "a+", stdout);
        
        if (execl(path,tracee_name,NULL) < 0){
            perror("can't execute the tracee");
            exit(1);
            
        }else{
            fclose(stdout);
            return 0;
        }

    }
    if (child_pid > 0) {    /* Parent process */
        if(mode == 's') return start_tracer_s(child_pid);
        else if (mode == 'p') return start_tracer_p(child_pid,tracee_name);
    }
    else {  /* Error accured */
        perror("problem for fork");
        exit(1);
    }
    
    return 0;
}

char * get_tracee_name(char *path){    
    int index=0;
    int flags = 0;  /* true path contains folders name */
    
    for(int i=0;path[i] != '\0';i++){
        if(path[i]== '/') { /* Pass each folder name */
            index = i;
            flags = 1;
        }
    }

    if (flags) 
        return path+index+1;
    else
        return path+index;
    
}