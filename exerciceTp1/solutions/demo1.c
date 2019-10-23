/*
 * Démonstration 1:
 * Nous allons créer un programme qui affiche le message "Hello INF2610" à
 * l'écran.
 */

#include <stdio.h>

int main(int argc, char** argv) {
    char course[] = "INF2610";
    printf("Hello %s\n", course);
    return 0;
}