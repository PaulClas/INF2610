/*
 * Init Lab - q2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <stdio.h>
#include <unistd.h>
// -------------------------------------------------

/*
 * Vous devez imprimer le message indiqué dans l'énoncé:
 * - En exécutant un premier appel à printf AVANT l'appel à write
 * - Sans utiliser la fonction fflush
 * - En terminant chaque ligne par le caractère '\n' de fin de ligne
 */
void question2()
{
    printf("2940c3e8384e (printed using printf)");
    write(1, "2940c3e8384e (printed using write)\n", 35);
    printf("\n");
}