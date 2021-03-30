#ifndef DICTIONNARY
#define DICTIONNARY
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct{
    char **label;
    unsigned long *address;
    unsigned long max_size;
    unsigned long size;
    
}Dic;

Dic *init_dic(void);

void free_dic(Dic *d);

bool add_el(Dic *d,unsigned long ad,char *lab);

char * get_label(Dic *d,unsigned long ad);

#endif