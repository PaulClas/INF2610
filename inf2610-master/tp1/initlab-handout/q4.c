/*
 * Init Lab - q4.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include <stdint.h>
#include <stdlib.h>
#include "q4/libq4.h"

// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

uint64_t question4A(uint64_t x) {
    return 21 * x*x + 10 * x + 4680;
}

void question4B(uint64_t x, uint64_t *result) {
    *result = question4A(x); 
}

uint64_t *question4C(uint64_t x) {
    uint64_t* result = malloc(sizeof(uint64_t));
    question4B(x, result);
    return result;
}

void question4D(uint64_t x, uint64_t **resultPtr) {
    uint64_t* result = question4C(x);
    *resultPtr = result;
}

/*
 * Attention: Vous devez impérativement obtenir le résultat du calcul de
 * l’expression demandée par un appel à la fonction _question4B (et non
 * question4B!) définie dans le fichier q4/libq4.h, que nous avons programmée
 * et qui a la même signature que votre fonction question4B.
 */
uint64_t question4E(uint64_t x) {
    uint64_t* result = malloc(sizeof(uint64_t));
    _question4B(x, result);
    return *result;
}