/*
 * Démonstration 3:
 * A savoir sur les pointeurs en C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    // Variables de taille connue à la compilation
    int number = 0;
    int numArray[7] = {1, 1, 2, 3, 5, 8, 0};

    // On peut modifier sa valeur directement
    number = 3;
    // Puis l'afficher
    printf("La variable number a pour valeur %d\n", number);

    // Et on peut aussi connaître l'adresse où elle est stockée en mémoire
    int* pointer = &number;
    // Le pointeur créé est de type int*: c'est un pointeur vers une variable
    // de type int, autrement dit une adresse mémoire qui désigne un
    // emplacement stockant un int.

    // On peut se servir du pointeur pour accéder à la valeur de la variable
    // number
    printf("La variable number a pour valeur %d (via le pointeur)\n", *pointer);

    // Le pointeur peut servir à modifier la variable désignée
    *pointer = 4;
    printf("La variable number a maintenant pour valeur %d\n", number);

    // En C, les tableaux sont naturellement des pointeurs
    *(numArray + 6) = *(numArray + 5) + *(numArray + 4);
    printf("fibo(7) = %d\n", numArray[6]);

    // Si on veut allouer dynamiquement des variables de taille non connue à la
    // compilation, on utilise malloc ou calloc
    char* word = calloc(numArray[6], sizeof(char));
    strcpy(word, "informatique");
    printf("l'%s c'est super\n", word);

    // C'est plus propre de libérer la mémoire ainsi allouée quand on ne s'en
    // sert plus
    free(word);

    return 0;
}