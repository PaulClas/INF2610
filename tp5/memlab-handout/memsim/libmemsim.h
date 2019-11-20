/*
 * Mem Lab - libmemsim.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _LIBMEMSIM_H
#define _LIBMEMSIM_H

#include <stdint.h>

/* Définition des structures */

struct memory_request {
    // ------------------------------------------------
    // Informations sur la requête d'accès à la mémoire
    // ------------------------------------------------
    /* Estampille de temps de la requête (commence à 0)
     * Vous utiliserez cette estampille de temps pour déterminer l'âge des
     * requêtes afin d'appliquer les politiques de remplacement */
    unsigned int timestamp;
    /* Adresse virtuelle demandée */
    unsigned long virtualAddr;
    // --------------------------------------------------
    // Champs à modifier lors du traitement de la requête
    // --------------------------------------------------
    /* Adresse physique correspondante */
    unsigned long physicalAddr;
    /* La traduction était-elle présente dans le TLB? */
    int wasFoundInTLB; // 0 = non; 1 = oui
    /* Y a-t-il eu un défaut de page (page absente de la mémoire?) */
    int wasPageFault; // 0 = non; 1 = oui
    /* Avez-vous du remplacer une entrée du TLB? */
    int wasEvictionInTLB; // 0 = non; 1 = oui
    /* Si une page a été remplacée dans le TLB, laquelle était-ce? */
    /* Donnez l'adresse virtuelle du DEBUT de la page remplacée dans le TLB */
    unsigned long virtualAddrEvictedFromTLB;
    /* La mémoire était-elle pleine? (auquel cas il a fallu remplacer une page
     * en mémoire) */
    int wasEvictionInMemory; // 0 = non; 1 = oui
    /* Si une page a été remplacée en mémoire, laquelle était-ce? */
    /* Donnez l'adresse virtuelle du DEBUT de la page remplacée en mémoire */
    unsigned long virtualAddrEvictedFromMemory;
};

#endif