#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEST_SIZE 50

int main() {

    /*
    char *prog_name = "hamza";
    char cmd[DEST_SIZE] ="nm ";
    strcat(cmd, prog_name);
    strcat(cmd," > nm_tracee_result.txt");
    int cr = system( cmd );
    if ( cr != 0 ) {
        fprintf( stderr, 
                 "Impossible de lancer la commande : %s\n",
                 cmd );
    }
    */

   
    
    
    FILE *f = fopen("nm_tracee_result.txt","r");

    
    

    



    return 0;
}