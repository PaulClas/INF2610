/*
 * Démonstration 2:
 * J'imprime deux chaînes de caractère, elles ne sont pas imprimées dans le bon
 * ordre... Pourquoi?
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    // Imprimer d'abord avec printf
    printf("printf dit bonjour\n");
    // Autre solution que \n: utiliser fflush(stdout);
    // Puis avec write...?
    write(STDOUT_FILENO, "write dit bonjour aussi\n", 24);
    return 0;
}