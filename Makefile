CC=gcc 
CFLAGS=-g --pedantic -Wall -Wextra -Wmissing-prototypes -std=c99 
PROGRAM=tracer 
# DO NOT MODIFY CC, CFLAGS and PROGRAM
# COMPLETE BELOW

all: $(PROGRAM)

tracer: hello.o tracee.o
	$(CC) --static -g -o tracer hello.o tracee.o

hello.o: hello.c
	$(CC) --static -g -o hello.o -c hello.c $(CFLAGS)

tracee.o: tracee.c hello.h
	$(CC) --static -g -o tracee.o -c tracee.c $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf hello