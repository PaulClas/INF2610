#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
 * Init Lab - q2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

/*
 * Vous devez imprimer le message indiqué dans l'énoncé:
 * - En exécutant un premier appel à printf AVANT l'appel à write
 * - Sans utiliser la fonction fflush
 * - En terminant chaque ligne par le caractère '\n' de fin de ligne
 */
void question2() {
    // TODO
    printf("091fa2cb557c (printed using printf)");
    write(1,"091fa2cb557c (printed using write)\n",strlen("091fa2cb557c (printed using write)\n"));
    printf("\n");
}