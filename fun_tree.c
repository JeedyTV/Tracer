#include "fun_tree.h"
#include <stdbool.h>
#include <stdio.h>



fun_tree *new_fun_tree(char *label, size_t depth, fun_tree *prev){
    fun_tree *tree = malloc(sizeof(fun_tree));
    if (!tree)
        return NULL;

    tree->label = label;
    tree->depth = depth;
    tree->nb_instructions = 0;
    tree->recursive = false;
    tree->nb_rec_calls = 0;
    tree->recursion_stage = 0;
    tree->prev = prev;
    tree->next = NULL;

    return tree;
}

void print_tree(fun_tree *tree){
    size_t d = tree->depth;
    while(d>0)
	printf("\t");
    printf("%s",tree->label);
    printf(": ");
    printf("%d",tree->nb_instructions);
    printf("\n");
    //TODO: if (tree->recursion)
}
void delete_fun_tree(fun_tree *tree){
    if (!tree)
	return;
    free(tree);
}
