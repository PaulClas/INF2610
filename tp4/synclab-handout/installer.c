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
    long packageNum;
    struct management_data *md;
    pthread_t id;
};

char *packageNames[10] = {
    "sumpoch-1.0",
    "librechong-3.14",
    "creng-2.12",
    "libvect-6.16",
    "wew-0.17",
    "spict-5.18",
    "libplosp-0.10",
    "skand-6.9",
    "honut-9.4",
    "libtizump-5.6"};
// -------------------------------------------------------

// TODO
// Si besoin, définissez ici des fonctions supplémentaires
// -------------------------------------------------------

// -------------------------------------------------------

/*
 * Cette fonction alloue et initialise certains champs de la structure md qui
 * sont utilisés dans le reste des fonctions.
 */
void initializeManagementData(struct management_data *md) {
    // TODO (Q2, Q3)
    md->downloadSemaphore = malloc(sizeof(sem_t));
    sem_init(md->downloadSemaphore, 0, 3);
    for (int i = 0; i < 10; ++i){
        md-> canInstallPackage[i]=malloc(sizeof(sem_t));
    }
    sem_init(md->canInstallPackage[0], 0, 0);
    sem_init(md->canInstallPackage[1], 0, 1);
    sem_init(md->canInstallPackage[2], 0, 0);
    sem_init(md->canInstallPackage[3], 0, 0);
    sem_init(md->canInstallPackage[4], 0, 0);
    sem_init(md->canInstallPackage[5], 0, 0);
    sem_init(md->canInstallPackage[6], 0, 1);
    sem_init(md->canInstallPackage[7], 0, 1);
    sem_init(md->canInstallPackage[8], 0, 1);
    sem_init(md->canInstallPackage[9], 0, 1);
}

void cleanUpThread(void* arg){
    struct threadArguments* newArg = (struct threadArguments*)(arg);
    doPackageCleanup(newArg->packageNum, newArg->md);

    switch(newArg->packageNum){
        case 9:
                pthread_cancel(newArg->md->tids[5]);
                pthread_cancel(newArg->md->tids[4]);
                pthread_cancel(newArg->md->tids[0]);
                break;
        case 8:
                pthread_cancel(newArg->md->tids[5]);
                pthread_cancel(newArg->md->tids[4]);
                break;
        case 7:
                pthread_cancel(newArg->md->tids[5]);
                pthread_cancel(newArg->md->tids[4]);
                break;
        case 6:
                pthread_cancel(newArg->md->tids[5]);
                pthread_cancel(newArg->md->tids[4]);
                pthread_cancel(newArg->md->tids[3]);
                break;
        case 5:
                pthread_cancel(newArg->md->tids[3]);
                break;
        case 4:
                pthread_cancel(newArg->md->tids[2]);
                pthread_cancel(newArg->md->tids[0]);
                break;
        case 3:
                pthread_cancel(newArg->md->tids[0]);
                break;
        case 2:
                pthread_cancel(newArg->md->tids[0]);
                break;
        case 1:
                pthread_cancel(newArg->md->tids[0]);
                break;
     }

}

/*
 * Cette fonction nettoie les champs de la structure md qui ont été initialisés
 * par la fonction initializeManagementData.
 */
void cleanupManagementData(struct management_data *md) {
    // TODO (Q2, Q3)
    sem_destroy(md->downloadSemaphore);
    free(md->downloadSemaphore);
    for (int i = 0; i < 10; ++i)
    {
        //free(md->canInstallPackage[i]);
        sem_destroy(md->canInstallPackage[i]);
    }
    
}

/*
 * Cette fonction télécharge et installe le paquet logiciel dont le numéro est
 * passé en argument.
 */
void installPackage(int packageNum, struct management_data *md) {
    // TODO (Q2, Q3)
    sem_wait(md->downloadSemaphore);
    doPackageDownload(packageNames[packageNum], md);
    sem_post(md->downloadSemaphore);

    sem_wait(md->canInstallPackage[packageNum]);
    doPackageInstall(packageNames[packageNum], md);
    
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
void wakePackage(int wokenPackageNum, struct management_data *md) {
    // TODO (Q3)
    if(md->isPackageInstalled[wokenPackageNum] == 0){
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
void postPackageInstall(int packageNum, struct management_data *md) {
    // TODO (Q3)
       md->isPackageInstalled[packageNum] = 1;

    if (md->isPackageInstalled[9] && md->isPackageInstalled[7] && md->isPackageInstalled[8] && md->isPackageInstalled[6]){
        wakePackage(4, md);
    }

    if (md->isPackageInstalled[9] && md->isPackageInstalled[7] && md->isPackageInstalled[8] && md->isPackageInstalled[6]){
        wakePackage(5, md);
    }
    
    if (md->isPackageInstalled[4]){
        wakePackage(2, md);
    }

    if(md->isPackageInstalled[6]){
        if (md->isPackageInstalled[5]){
        wakePackage(3, md);
        }
    }
    
    
    if (md->isPackageInstalled[1] && md->isPackageInstalled[2] && md->isPackageInstalled[3] && md->isPackageInstalled[9] && md->isPackageInstalled[4])
    {
        wakePackage(0, md);
    }
   
   
}

void *threadWrapper(void *args)
{
    struct threadArguments *argss = (struct threadArguments *)args;
    pthread_cleanup_push(cleanUpThread, argss);
    threadedPackageInstaller(argss->id, argss->md);
    pthread_cleanup_pop(0);
    //free(args);
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
void installer(struct management_data *md) {
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
        args->packageNum = i; 
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