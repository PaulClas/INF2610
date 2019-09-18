#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
/*
 * Init Lab - q3.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

/*
 * Vous devez imprimer dans le fichier indiqué dans l'énoncé le message suivant:
 *  
 * This file has been opened by process ID CURRENT_PID.
 * 
 * - En terminant le message par le caractère '\n' de fin de ligne
 * - En remplaçant CURRENT_PID par le PID du processus qui exécutera votre solution
 */
/*void question3() {
    // TODO
    const char* filename = "q3Output-def7dda54c92.txt";
    FILE* file = fopen(filename, "w");
    //String phrase = "This file has been opened by process ID %d\n", getpid();
    //write(file,phrase,strlen("This file has been opened by process ID %d\n"));
    fprintf(file,"This file has been opened by process ID %d.\n", getpid());
    fclose(file);
}*/

void question3(){
    char f[1000];
    int file = open("q3Output-def7dda54c92.txt", O_WRONLY);
    sprintf(f,"This file has been opened by process ID %d.\n",getpid());
    write(file,f, strlen(f));
    close(file);

    
}