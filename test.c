#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionnary.h"

#define DEST_SIZE 50

int main() {

    char *prog_name = "tracee";
    char cmd[DEST_SIZE] ="nm ";
    strcat(cmd, prog_name);
    strcat(cmd," > nm_tracee_result");
    int cr = system( cmd );
    if ( cr != 0 ) fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);
    
    cr = system("wc -l nm_tracee_result > nbre_ligne");
    if ( cr != 0 ) fprintf( stderr,"Impossible de lancer la commande : wc\n");
    
    FILE *fichier = fopen("nbre_ligne","r");
    int nbr_line;
    
    if (fichier != NULL){
        fscanf(fichier, "%d", &nbr_line);

        cr = system("rm nbre_ligne");
        if ( cr != 0 ) fprintf( stderr,"Impossible de lancer la commande : rm\n");

        fclose(fichier);

    }
    
    fichier = fopen("nm_tracee_result","r");
    unsigned long ip;
    char trash[30];
    char label[30];
    Dic * d = init_dic();
    
    if(d == NULL) return 1;
    
    if (fichier != NULL){

        for(int i=0;i<nbr_line;i++){

            if(fscanf(fichier, "%08lx %s %s", &ip,trash, label)){
                if(!add_el(d,ip,label)) return 1;

            }  
            else{
                fscanf(fichier, "%s %s",trash, label);
            } 
            
        }
        
        fclose(fichier);
    }

    printf("label of %08lx is %s \n",0x080480b8,get_label(d,0x080480b8));
    
    free_dic(d);

    cr = system("rm nm_tracee_result");
    if ( cr != 0 ) fprintf( stderr, "Impossible de lancer la commande : %s\n", cmd);

    return 0;
}