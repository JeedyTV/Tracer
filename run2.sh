#!/bin/bash

#gcc test2.S -g -o tracee --static -nostdlib -Wl,-entry="main"


gcc -g test2.c --static  -o tracee
#gcc -g jd_main.c dictionnary.c stack.c sys_call.c --static -o out
gcc -g main.c fun_tree.c dictionnary.c stack.c sys_call.c --static -o out
#gcc -g test.c dictionnary.c sys_call.c --static -o out

#valgrind -v ./out tracee
./out tracee