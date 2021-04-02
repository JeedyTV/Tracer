// inspired  by https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/

// C program for array implementation of stack

#include "stack.h"


Stack* createStack(unsigned capacity)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    
    assert( stack != NULL );
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (fun_tree**)malloc(stack->capacity * sizeof(fun_tree*));
    
    assert(  stack->array != NULL );
    return stack;
}
 
// Stack is full when top is equal to the last index
int isFull(Stack* stack)
{
    return stack->top == stack->capacity - 1;
}
 
// Stack is empty when top is equal to -1
int isEmpty(Stack* stack)
{
    return stack->top == -1;
}
 
// Function to add an item to stack.  It increases top by 1
void push(Stack* stack, fun_tree* item)
{
    if (isFull(stack)){

        
        fun_tree* *tab = (fun_tree**)malloc(stack->capacity * 2 *sizeof(fun_tree*));
        
        assert( tab != NULL );
        for(int i=0;i<stack->capacity;i++){
            tab[i] = stack->array[i];
        }
        free(stack->array);
        
        stack->array = tab;
        stack->array[++stack->top] = item;
        stack->capacity *= 2;

    }
    else{
        stack->array[++stack->top] = item;
    }
    
}
 
// Function to remove an item from stack.  It decreases top by 1
fun_tree* pop(Stack* stack)
{
    if (isEmpty(stack))
        return NULL;
    return stack->array[stack->top--];
}

fun_tree* peak(Stack* stack)
{
    if (isEmpty(stack))
        return NULL;
    return stack->array[stack->top];
}

void free_stack(Stack *s){

    free(s->array);
    
    free(s);
    
    
}