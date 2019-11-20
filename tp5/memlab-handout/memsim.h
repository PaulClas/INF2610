/*
 * Mem Lab - memsim.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _MEMSIM_H
#define _MEMSIM_H

#include "memsim/libmemsim.h"

/* -------------------------------------------------------------------------
 * Définissez ici vos structures de données
 * -------------------------------------------------------------------------
 * Nous vous proposons des structures de données prêtes à être utilisées
 * pour sauver l'état de la mémoire. Vous êtes libres de les modifier ou
 * non. Si vous choisissez de les utiliser telles quelles, aidez-vous des
 * commentaires ci-dessous pour comprendre le rôle de chaque champ de chaque
 * structure.
 * ------------------------------------------------------------------------- */

/*
 * La structure 'struct tlb' sauve l'état courant du Translation Lookaside
 * Buffer. Il s'agit en pratique d'un tableau avec un nombre de lignes fixe (le
 * nombre d'entrées disponibles dans le TLB) et plusieurs colonnes qui
 * décrivent l'entrée considérée dans le TLB:
 *  -----------------------------------------------------------------------
 * | Page Number | Frame number | Last access time | Creation time | Used? |
 *  -----------------------------------------------------------------------
 */
struct tlb {
    // La valeur à l'indice i du tableau pageNumbers correspond au numéro de
    // page de l'entrée numéro i du TLB.
    unsigned long* pageNumbers;
    // La valeur à l'indice i du tableau frameNumbers correspond au numéro de
    // cadre de l'entrée numéro i du TLB.
    unsigned long* frameNumbers;
    // La valeur à l'indice i du tableau lastAccessTimestamps correspond à
    // l'estampille de temps correspondant au dernier accès à l'entrée numéro i
    // du TLB. Pour simplifier, l'estampille de temps est simplement le numéro
    // de la requête qui a provoqué l'accès.
    // Très utile pour une politique LRU.
    unsigned int* lastAccessTimestamps;
    // La valeur à l'indice i du tableau entryCreationTimestamps correspond à
    // l'estampille de temps correspondant à la création de l'entrée numéro i
    // du TLB.
    // Très utile pour une politique FIFO.
    unsigned int* entryCreationTimestamps;
    // La valeur à l'indice i du tableau isUsed permet d'indiquer si l'entrée i
    // du TLB est utilisée (valeur 1) ou non (valeur 0).
    char* isUsed;
};

/*
 * La structure 'struct pt' sauve l'état courant de la table de pages
 * considérée. Il s'agit en pratique d'un tableau avec un nombre de lignes
 * fixe (le nombre maximal de pages utilisées) et plusieurs colonnes qui
 * décrivent l'entrée considérée dans la table de pages:
 *  --------------------------
 * | Frame Number | Is Valid? |
 *  --------------------------
 */
struct pt {
    // La valeur à l'indice i du tableau frameNumbers fournit le numéro de cadre
    // correspondant à la page numéro i (si la traduction est marquée comme
    // valide).
    unsigned long* frameNumbers;
    // La valeur à l'indice i du tableau isValid permet d'indiquer si la
    // traduction fournie est valide (valeur 1) ou non (valeur 0).
    char* isValid;
};

/*
 * La structure 'struct memory' sauve l'état courant de la mémoire physique.
 * Il s'agit en pratique d'un tableau avec un nombre de lignes fixe (le nombre
 * de cadres disponibles en mémoire) et plusieurs colonnes qui décrivent
 * l'entrée considérée:
 *  --------------------------------------------------------
 * | Page Number | Last access time | Creation time | Used? |
 *  --------------------------------------------------------
 * ATTENTION: Si le premier numéro de cadre disponible est pas exemple le cadre
 * 197, alors l'indice numéro 0 du tableau correspond au cadre n°197, l'indice
 * numéro 2 au cadre n°198, etc.
 */
struct memory {
    // La valeur à l'indice i du tableau pageNumbers correspond au numéro de
    // la page stockée dans le cadre décrit par l'entrée n°i.
    unsigned long* pageNumbers;
    // La valeur à l'indice i du tableau lastAccessTimestamps correspond à
    // l'estampille de temps du dernier accès au cadre.
    unsigned int* lastAccessTimestamps;
    // La valeur à l'indice i du tableau entryCreationTimestamps correspond à
    // l'estampille de temps de la copie en mémoire de la page stockée dans le
    // cadre.
    unsigned int* entryCreationTimestamps;
    // La valeur à l'indice i du tableau isUsed permet d'indiquer si le cadre
    // décrit par l'entrée n°i est utilisé (valeur 1) ou non (valeur 0).
    char* isUsed;
};

/*
 * La structure 'struct paging_system_state' permet d'encapsuler l'état du TLB,
 * de la table de pages et de la mémoire dans une seule variable. Elle contient
 * simplement un pointeur vers chacune des structures décrites plus haut.
 */
struct paging_system_state {
    struct tlb* tlb;
    struct pt* pt;
    struct memory* mem;
};
/* ------------------------------------------------------------------------- */

struct paging_system_state* initMemoryState();
void processMemoryRequest(struct paging_system_state *ms,
                          struct memory_request *mr);
void cleanMemoryState(struct paging_system_state* ms);

unsigned long getPageOffset(unsigned long addr);
unsigned long getPageNumber(unsigned long addr);
unsigned long getStartPageAddress(unsigned long pageNumber);

#endif
