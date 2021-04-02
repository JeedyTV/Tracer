#ifndef STACK
#define STACK
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fun_tree.h"

typedef struct  {
    int top;
    int capacity;
    fun_tree** array;
}Stack;

Stack* createStack(unsigned capacity);

int isFull(Stack* stack);

int isEmpty(Stack* stack);

void push(Stack* stack, fun_tree* item);

fun_tree* pop(Stack* stack);

fun_tree* peak(Stack* stack);

void free_stack(Stack *s);

#endif