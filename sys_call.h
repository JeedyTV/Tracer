#ifndef SYSCALL
#define SYSCALL


char ** get_link(void);
void free_link(char ** tab);
void trace_syscall(char *path);
char * get_process_name(char *path);

#endif
