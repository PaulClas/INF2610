/*
 * Sync Lab - installer.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include "libsynclab.h"
#include "installer/libinstaller.h"

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <semaphore.h>
// -------------------------------------------------

// TODO
// Si besoin, définissez ici des types de structures et/ou
// des variables
// -------------------------------------------------------
struct threadArguments
{
    struct management_data *md;
    pthread_t id;
};

char *packageNames[10] = {
    "jonk-6.0",
    "libsiwemp-0.9",
    "dod-1.1",
    "duw-9.7",
    "libgrundaft-3.12",
    "shuct-8.16",
    "libdoy-5.17",
    "libspilomp-3.9",
    "libchozisp-1.1",
    "rop-6.14"};

// -------------------------------------------------------

// TODO
// Si besoin, définissez ici des fonctions supplémentaires
// -------------------------------------------------------

// -------------------------------------------------------

/*
 * Cette fonction alloue et initialise certains champs de la structure md qui
 * sont utilisés dans le reste des fonctions.
 */
void initializeManagementData(struct management_data *md)
{
    md->downloadSemaphore = malloc(sizeof(sem_t));
    for (int i = 0; i < 10; i++)
    {
        md->canInstallPackage[i] = malloc(sizeof(sem_t));
        if (i > 6)
        {
            sem_init(md->canInstallPackage[i], 0, 1);
        }
        else
        {
            sem_init(md->canInstallPackage[i], 0, 0);
        }
    }

    sem_init(md->downloadSemaphore, 0, 3);
}

void cleanUpThread(){

}

/*
 * Cette fonction nettoie les champs de la structure md qui ont été initialisés
 * par la fonction initializeManagementData.
 */
void cleanupManagementData(struct management_data *md)
{
    sem_destroy(md->downloadSemaphore);
    free(md->downloadSemaphore);
    for (int i = 0; i < 10; i++)
    {
        free(md->canInstallPackage[i]);
    }
    // TODO (Q2, Q3)
}

/*
 * Cette fonction télécharge et installe le paquet logiciel dont le numéro est
 * passé en argument.
 */
void installPackage(int packageNum, struct management_data *md)
{
    sem_wait(md->downloadSemaphore);
    doPackageDownload(packageNames[packageNum], md);
    sem_post(md->downloadSemaphore);

    sem_wait(md->canInstallPackage[packageNum]);
    doPackageInstall(packageNames[packageNum], md);
    // TODO (Q2, Q3)
}

/*
 * Cette fonction vérifie si le paquet logiciel dont le numéro est passé en
 * argument est prêt pour l'installation. Si c'est le cas, la fonction débloque
 * le paquet pour que son installation puisse se lancer.
 * 
 * NOTE: Cette fonction vous aidera à clarifier votre code pour la fonction
 * postPackageInstall. Il est fortement recommandée de l'utiliser, mais ce
 * n'est pas obligatoire.
 */
void wakePackage(int wokenPackageNum, struct management_data *md)
{
    // TODO (Q3)
    if (md->isPackageInstalled[wokenPackageNum] == 0)
    {
        sem_post(md->canInstallPackage[wokenPackageNum]);
    }
}

/*
 * Cette fonction est exécutée après l'installation du paquet logiciel dont
 * le numéro est passé en argument. Son rôleF est de marquer le paquet comme
 * installé dans la structure md, et également de débloquer l'installation des
 * paquets logiciels qui pourraient désormais être prêts pour installation.
 * 
 * Afin de clarifier votre code, il est conseillé de compléter et d'utiliser la
 * fonction wakePackage définie juste au-dessus.
 * 
 * ATTENTION: Cette fonction est testée de manière unitaire par le script de
 * notation. Vous devez vous assurer qu'elle a bien le comportement décrit plus
 * haut et qu'elle ne modifie pas d'autres variables que celles stockées dans
 * la structure md.
 */
void postPackageInstall(int packageNum, struct management_data *md)
{
    // TODO (Q3)
    md->isPackageInstalled[packageNum] = 1;

    if (md->isPackageInstalled[8])
    {
        wakePackage(4, md);
        if (md->isPackageInstalled[9])
        {
            wakePackage(5, md);
            if (md->isPackageInstalled[7])
            {
                wakePackage(6, md);
            }
        }
    }

    if (md->isPackageInstalled[6])
    {
        if (md->isPackageInstalled[5])
        {
            wakePackage(1, md);
            wakePackage(2, md);
        }
        if (md->isPackageInstalled[4])
        {
            wakePackage(3, md);
        }
    }

    if (md->isPackageInstalled[1] && md->isPackageInstalled[2] && md->isPackageInstalled[3])
    {
        wakePackage(0, md);
    }
}

void *threadWrapper(void *args)
{
    struct threadArguments *argss = (struct threadArguments *)args;
    
    threadedPackageInstaller(argss->id, argss->md);
    free(args);
    return 0;
}

/*
 * Cette fonction crée les dix fils d'exécution qui seront chargés de
 * l'installation des paquets logiciels.
 * Chaque fil d'exécution doit exécuter la fonction threadedPackageInstaller
 * en lui passant comme argument:
 * - le numéro du paquet logiciel à installer (compris entre 0 et 9)
 * - le pointeur md vers la structure passée en argument de la fonction
 *   installer
 */
void installer(struct management_data *md)
{
    // NE PAS MODIFIER
    // --------------------------
    initializeManagementData(md);
    installerTestInit(md);
    // --------------------------

    // TODO (Q1)
    // Lancer les fils d'exécution
    // ---------------------------
    for (int i = 0; i < 10; i++)
    {
        struct threadArguments *args = malloc(sizeof(struct threadArguments));
        args->id = i;
        args->md = md;
        pthread_create(&md->tids[i], NULL, threadWrapper, (void *)args);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(md->tids[i], NULL);
    }

    // ---------------------------

    // NE PAS MODIFIER
    // -----------------------
    cleanupManagementData(md);
    // -----------------------
}