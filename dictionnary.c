#include "dictionnary.h"

Dic *init_dic(void){
  
    Dic * d = malloc(sizeof(Dic));

    d->size = 0;
    d->max_size = 1;
    
    d->label = malloc(sizeof(char*)*d->max_size);
    if(d->label == NULL) return NULL;
        
    d->address = malloc(sizeof(unsigned long)*d->max_size);
    if(d->label == NULL) return NULL;
    

    return d;

}

bool add_el(Dic *d,unsigned long ad,char *lab){
  
    if(d->size < d->max_size){
    
        d->address[d->size] = ad;
        d->label[d->size] = malloc(sizeof(lab));
        if(d->label == NULL) return false;
        strcpy(d->label[d->size],lab);
        d->size ++;
        return true;
  
    }else{
    
        d->max_size *= 2;
    
        unsigned long* a = malloc(sizeof(unsigned long)*d->max_size);
        if(a == NULL) return false;
        
        for(int i=0;i<d->size;i++){
            a[i] = d->address[i];
        }
        
        d->address = a;
        
        d->address[d->size] = ad;
        
        char** l = malloc(sizeof(char*)*d->max_size);
        if(l == NULL) return false;
        
        for(int i=0;i<d->size;i++){
            l[i] = d->label[i];
        }
    
        d->label = l;
        
        d->label[d->size] = malloc(sizeof(lab));
        if(d->label[d->size] == NULL) return false;
        strcpy(d->label[d->size],lab);
        
        d->size ++;
        return true;
    
    }
  

}

void free_dic(Dic *d){
  
    for(int i=0;i<d->size-1;i++){
        free(d->label[i]);
    }
    free(d->label);
    free(d->address);
    free(d);

}

char * get_label(Dic *d,unsigned long ad){
  
    for(int i = 0;i<d->size;i++){
        
        if(d->address[i] == ad) return d->label[i];
    }
    
    return NULL;

}