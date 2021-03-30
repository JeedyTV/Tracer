CC=gcc 
CFLAGS=-g --pedantic -Wall -Wextra -Wmissing-prototypes -std=c99 
PROGRAM=tracer 
# DO NOT MODIFY CC, CFLAGS and PROGRAM
# COMPLETE BELOW

all: $(PROGRAM)

tracer: sys_call.o tracee.o dictionnary.o stack.o
	$(CC) --static -g -o tracer sys_call.o tracee.o dictionnary.o stack.o

sys_call.o: sys_call.c
	$(CC) --static -g -o sys_call.o -c sys_call.c $(CFLAGS)

dictionnary.o: dictionnary.c
	$(CC) --static -g -o dictionnary.o -c dictionnary.c $(CFLAGS)

stack.o: stack.c
	$(CC) --static -g -o stack.o -c stack.c $(CFLAGS)

tracee.o: tracee.c sys_call.h
	$(CC) --static -g -o tracee.o -c tracee.c $(CFLAGS)

clean:
	rm -rf *.o

comp:
	@rm -f src.tar.xz
	@tar -cJf src.tar.xz *.c *.h