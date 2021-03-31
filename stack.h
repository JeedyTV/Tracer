#ifndef STACK
#define STACK
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct  {
    int top;
    unsigned capacity;
    unsigned long* array;
}Stack;

Stack* createStack(unsigned capacity);

int isFull(Stack* stack);

int isEmpty(Stack* stack);

void push(Stack* stack, int item);

int pop(Stack* stack);

int peak(Stack* stack);

void free_stack(Stack *s);

#endif