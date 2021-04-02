#include "dictionnary.h"



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
 
    if(d == NULL)return;
  
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
  
    if(d == NULL) return NULL;
    
    Node *n = d->head;

    char * label = NULL;
    
    while(n!= NULL){
        if(ad == n->address) label = n->label;
        n = n->precedent;
    }

    if(label) return label;
    else return NULL;
}