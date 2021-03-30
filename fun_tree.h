#ifndef FUN_TREE_H
#define FUN_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
typedef struct fun_tree_t{
    char *label;
    size_t depth;
    size_t nb_instructions;
    bool recursive;
    size_t nb_recursions;
    struct fun_tree_t *parent;
    struct fun_tree_t *next;
} fun_tree;

fun_tree *new_fun_tree(char *label, size_t depth, fun_tree *parent);

//void update_parent(fun_tree *tree);

void print_tree(fun_tree *tree);

void delete_fun_tree(fun_tree *tree);  
#endif
