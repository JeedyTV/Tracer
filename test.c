#include<stdlib.h>
//#include <errno.h>
#include <sys/syscall.h>
//#include <sys/types.h>
#include <unistd.h>

void print(){
 syscall(SYS_write,1,"Hello world\n", 12);
 return;
}
void rec(int i){
 if(i != 0)
 rec(i - 1);
 print();
 return;
}
int main(){
    rec(4);
    exit(0);
}