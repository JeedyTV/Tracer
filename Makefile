CC=gcc 
CFLAGS=-g --pedantic -Wall -Wextra -Wmissing-prototypes -std=c99 
PROGRAM=tracer 
# DO NOT MODIFY CC, CFLAGS and PROGRAM
# COMPLETE BELOW

all: $(PROGRAM)

tracer: sys_call.o tracee.o dictionnary.o stack.o fun_tree.o profiler.o tracer.o
	$(CC) --static -g -o tracer sys_call.o tracee.o dictionnary.o stack.o fun_tree.o profiler.o tracer.o

sys_call.o: sys_call.c 
	$(CC) --static -g -o sys_call.o -c sys_call.c $(CFLAGS)

dictionnary.o: dictionnary.c
	$(CC) --static -g -o dictionnary.o -c dictionnary.c $(CFLAGS)

stack.o: stack.c
	$(CC) --static -g -o stack.o -c stack.c $(CFLAGS)

fun_tree.o: fun_tree.c
	$(CC) --static -g -o fun_tree.o -c fun_tree.c $(CFLAGS)

tracer.o: tracer.c sys_call.h profiler.h
	$(CC) --static -g -o tracer.o -c tracer.c $(CFLAGS)

profiler.o: profiler.c stack.h fun_tree.h dictionnary.h
	$(CC) --static -g -o profiler.o -c profiler.c $(CFLAGS)

tracee.o: tracee.c tracer.h
	$(CC) --static -g -o tracee.o -c tracee.c $(CFLAGS)

clean:
	rm -rf *.o

comp:
	@./make_archive.sh