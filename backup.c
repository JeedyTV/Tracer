#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/reg.h>

int waitchild(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    if(WIFSTOPPED(status)) {
        return 0;
    }
    else if (WIFEXITED(status)) {
        return 1;
    }
    else {
        printf("%d raised an unexpected status %d", pid, status);
        return 1;
    }
}

void trace(pid_t child) {
  unsigned long instruction, ip;
  //struct user_regs_struct regs;
  do {
    ip = ptrace(PTRACE_PEEKUSER, child, 4 * EIP, NULL);
    instruction = ptrace(PTRACE_PEEKTEXT, child, ip, NULL);
    printf("--%08lx--,--%08lx--\n",instruction,ip);
    ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
  } while(waitchild(child) < 1);
}

int main(int argc, char ** argv) {
    
    pid_t child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl(argv[1],argv[1],NULL);
    }
    else {
        // wait for the child to stop
        waitchild(child);
        trace(child);
    }
    return 0;
}