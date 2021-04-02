#include "fun_tree.h"
#include <stdbool.h>
#include <stdio.h>


fun_tree *new_fun_tree(char *label, size_t depth, fun_tree *prev,unsigned long ret_ad){
    fun_tree *tree = (fun_tree *)malloc(sizeof(fun_tree));
    
    assert( tree != NULL);

    tree->label = label;
    tree->depth = depth;
    tree->nb_instructions = 0;
    tree->recursive = false;
    tree->nb_rec_calls = 0;
    tree->recursion_stage = 0;
    tree->prev = prev;
    tree->sub_size_max = 1000;
    tree->subtree =  (fun_tree **)malloc(sizeof(fun_tree *)*tree->sub_size_max);
    
    tree->sub_size = 0;
    tree->return_address = ret_ad;

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
    
    for(int i = 0 ; i<tree->sub_size;i++){
        print_tree(tree->subtree[i]);
    }


    
}

void delete_fun_tree(fun_tree *tree){
    if (!tree)
	    return;

    for(int i =0;i<tree->sub_size;i++){
        delete_fun_tree(tree->subtree[i]);
    }
    free(tree->prev);
    

    free(tree);
    
}

// To add an element at the end
void add(fun_tree *tree, fun_tree *element){
    
    if (tree->sub_size == tree->sub_size_max){

        fun_tree **tab = (fun_tree **)malloc(sizeof(fun_tree *)*tree->sub_size_max*2);
        
        assert( tab != NULL );
        for(int i=0;i<tree->sub_size_max;i++){
            tab[i] = tree->subtree[i];
        }
        free(tree->subtree);
        
        tree->subtree = tab;
        tree->subtree[tree->sub_size] = element;
        tree->sub_size++;
        tree->sub_size_max *= 2;

    }
    else{
        tree->subtree[tree->sub_size] = element;
        tree->sub_size++;
    }
}
// To add a recursion
void add_rec(fun_tree *tree){
    assert(tree != NULL);
    tree->recursive = true;
    tree->nb_rec_calls++;
    tree->recursion_stage++;
}
