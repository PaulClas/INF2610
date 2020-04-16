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

// -------------------------------------------------

// TODO
// Si besoin, définissez ici des types de structures et/ou
// des variables
// -------------------------------------------------------

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
    
}

/*
 * Cette fonction nettoie les champs de la structure md qui ont été initialisés
 * par la fonction initializeManagementData.
 */
void cleanupManagementData(struct management_data *md) {
    // TODO (Q2, Q3)
    
}

/*
 * Cette fonction télécharge et installe le paquet logiciel dont le numéro est
 * passé en argument.
 */
void installPackage(int packageNum, struct management_data *md) {
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
void wakePackage(int wokenPackageNum, struct management_data *md) {
    // TODO (Q3)
    
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
    
    // ---------------------------

    // NE PAS MODIFIER
    // -----------------------
    cleanupManagementData(md);
    // -----------------------
}