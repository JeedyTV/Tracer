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
    if (!tree)
        return;
    for (size_t d = 0; d < tree->depth*4; d++)
        printf(" ");

    if(!tree->recursive)
        printf("%s: %u\n", tree->label, tree->nb_instructions);
    else
        printf("%s [rec call: %u]: %u\n", tree->label, tree->nb_rec_calls, tree->nb_instructions);
    
    fun_tree *subtree = tree->subtree;
    while(subtree){
        print_tree(subtree);
        subtree = subtree->next;
    }

    if(tree->next)
        print_tree(tree->next);
}

void delete_fun_tree(fun_tree *tree){
    if (!tree)
	    return;

    fun_tree *subtree = tree->subtree;
    while(subtree){
        fun_tree *tmp = subtree->next;
        delete_fun_tree(subtree);
        subtree = tmp;
    }

    if(tree->next)
        delete_fun_tree(tree->next);

    free(tree);
}
