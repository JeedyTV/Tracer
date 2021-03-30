#ifndef FUN_TREE_H
#define FUN_TREE_H

#include <stdlib.h>

typedef struct fun_tree_t fun_tree;

fun_tree* new_fun_tree(char *label, size_t depth, fun_tree *parent);

void update_parent(fun_tree tree);

char* tree_to_string(fun_tree tree);

void delete_fun_tree(fun_tree *tree);  
#endif