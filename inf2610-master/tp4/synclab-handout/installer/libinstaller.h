/*
 * Sync Lab - libinstaller1.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _LIBINSTALLER_H
#define _LIBINSTALLER_H

#include <pthread.h>
#include <semaphore.h>

/* Définition des structures */

struct management_data {
    /* Liste des threads pour la question 1 */
    pthread_t tids[10];
    /* Sémaphore pour la question 2 */
    sem_t* downloadSemaphore;
    /* Sémaphores pour la question 3 */
    sem_t* canInstallPackage[10];
    /* Information sur l'état des paquets pour la question 3 */
    int isPackageInstalled[10];
    /* NE PAS MODIFIER LA VALEUR DE lab_mngmt */
    /* -------------------------------------- */
    void* lab_mngmt;
    /* -------------------------------------- */
 };

/* Fonctions */

void* threadedPackageInstaller(int packageNum, struct management_data *md);
void doPackageDownload(char *packageName, struct management_data *md);
void doPackageInstall(char *packageName, struct management_data *md);
void doPackageCleanup(int packageNum, struct management_data *md);

#endif