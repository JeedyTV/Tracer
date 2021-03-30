#include "fun_tree.h"
#include <stdbool.h>

typedef struct fun_tree_t{
    char* label;
    size_t depth;
    size_t nb_instructions;
    bool recursive;
    size_t nb_recursions;
    fun_tree* parent;
    fun_tree* next;
} fun_tree;

fun_tree* new_fun_tree(char *label_t, size_t depth_t, fun_tree *parent_t){
    fun_tree *tree = malloc(sizeof(fun_tree));
    if (!tree)
        return NULL;

    tree->parent = parent_t;
    tree->depth = depth_t;
    tree->nb_instructions = 0;
    tree->recursive = false;
    tree->nb_recursions = 0;
    tree->parent = parent_t;
    tree->next = NULL;

    return tree;
}

char* tree_to_string(fun_tree tree){
    char* to_print;
}
void delete_fun_tree(fun_tree *tree){
    if (!tree)
	return;
    free(tree);
}
