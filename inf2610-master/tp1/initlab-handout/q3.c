/*
 * Init Lab - q3.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
// -------------------------------------------------

/*
 * Vous devez imprimer dans le fichier indiqué dans l'énoncé le message suivant:
 *  
 * This file has been opened by process ID CURRENT_PID.
 * 
 * - En terminant le message par le caractère '\n' de fin de ligne
 * - En remplaçant CURRENT_PID par le PID du processus qui exécutera votre solution
 */
void question3() {
    const char* fileName = "q3Output-05d448c2f2cd.txt";
    FILE* file = fopen(fileName, "w");
    fprintf(file, "This file has been opened by process ID %d.\n", getpid());

    fclose(file);
}