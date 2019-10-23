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
    // Puis avec write...?
    write(STDOUT_FILENO, "write dit bonjour aussi", 23);
    return 0;
}