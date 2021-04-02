/** @file dictionnary.h
 *  A dictionnary data structure.
 */

#ifndef DICTIONNARY
#define DICTIONNARY
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


typedef struct Node_t{
    char *label;
    unsigned long address;
    struct Node_t *precedent;  
}Node;


typedef struct {
    unsigned long size;
    Node *head;  
}Dic;

Dic *init_dic(void);

void free_dic(Dic *d);

bool add_el(Dic *d,unsigned long ad,char *lab);

char * get_label(Dic *d,unsigned long ad);


#endif