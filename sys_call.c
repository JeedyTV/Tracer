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

int wait_syscall(pid_t child){

    int status;
    int error = 0;
    
    while(1){

        error = ptrace(PTRACE_SYSCALL, child, 0,0);
        if(error == -1){
            return error;
        }

        waitpid(child,&status,0);


        if(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80){
            return 0;
        }

        if (WIFEXITED(status)){
            return 1;
        }
    }

    return 0;
}

int start_tracer_s(pid_t child){
        
    char ** syscall = get_link();   /* contains syscal names */
    if(syscall == NULL) return 1;
    
    int status;     /* for the wait */
    int error= 0;
    int sys_call;
    int ret;

    error = waitpid(child,&status,0);
    if(error == -1){
        return error;
    }

    error = ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
	if (error == -1) {
		return error;
	}

    sys_call = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_EAX);
    printf("syscall: %s\n",syscall[sys_call]);

    while (1)
    {
        error = wait_syscall(child);
        if(error != 0){
            break;
        }

        sys_call = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_EAX);
        printf("syscall: %s\n",syscall[sys_call]);

        error = wait_syscall(child);
		if (error != 0) {
			break;
		}

        ptrace(PTRACE_PEEKUSER, child, sizeof(long) * EAX);
		

    }

    free_link(syscall);

    return 0;
}


