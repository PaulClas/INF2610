/*
 * Init Lab - q1.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <stdio.h>
// -------------------------------------------------

/*
 * Le programme doit imprimer sur la sortie standard le nombre d'arguments
 * qu'il a reçus et la liste de ces arguments, selon le format suivant:
 * 
 * Exécution en ligne de commande:
   $ ./q1 monargument1 2 monargument3
 * Sortie attendue:
   Program received 4 arguments:
   ./q1
   monargument1
   2
   monargument3
 * 
 * Attention:
 * - Chaque ligne doit être terminée par le caractère '\n' de fin de ligne
 * - Si le programme ne reçoit qu'un seul argument, la première ligne affichée
 *   par le programme doit être "Program received 1 argument:", sans 's' à la
 *   fin de "argument"
 */
int main(int argc, char* argv[]) {
    
    char* endChar = argc > 1 ? "s" : "";
    printf("Program received %d argument%s:\n", argc, endChar);
    for(int i=0; i< argc; i++){
      
      printf("%s\n", argv[i]);
    }
    return 0;
}