#include "dictionnary.h"

typedef struct Node_{
    char *label;
    unsigned long address;
    Node *precedent;  
}Node;

typedef struct Dic_{
    unsigned long size;
    Node *head;  
}Dic;

Dic *init_dic(void){
  
    Dic * d = (Dic*)malloc(sizeof(Dic));
    assert( d != NULL );
    
    d->head = NULL;

    d->size = 0;
        
    return d;

}

bool add_el(Dic *d,unsigned long ad,char *lab){
    
    if(d->size ==0){
        Node *n = (Node*)malloc(sizeof(Node));
        assert( n != NULL );
        size_t l = strlen( lab ) + 1;
        n->label = (char *) malloc( l * sizeof(char) );
        assert( n->label != NULL );
        strcpy(n->label,lab);
        n->address = ad;
        n->precedent = NULL;
        d->head = n;
        d->size++;
    
    }else{
        Node *n = (Node*)malloc(sizeof(Node));
        assert( n != NULL );
        size_t l = strlen( lab ) + 1;
        n->label = (char *) malloc( l * sizeof(char) );
        assert( n->label != NULL );
        strcpy(n->label,lab);
        n->address = ad;
        n->precedent = d->head;
        
        d->head = n;
        d->size++;
    
    }
    
    return true;

}

void free_dic(Dic *d){
  
    Node *n = d->head;
        
    while(n != NULL){
        free(n->label);
        Node *temp = n; 
        n = n->precedent;
        free(temp);

    }
  
    free(d);
}

char * get_label(Dic *d,unsigned long ad){
  
    Node *n = d->head;
    
    while(n!= NULL){
        if(ad == n->address) return n->label;
        n = n->precedent;
    
    }
    
    return NULL;
}