/*
 * Clone Lab - part1.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */


// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include  <stdio.h>
#include  <sys/types.h>
#include  <unistd.h>
#include  <stdlib.h>

// -------------------------------------------------

void part1() {

    pid_t pidPere = getpid();
    pid_t pidFils = fork();
    
    char* pidStr = malloc(sizeof(pid_t));// afin de transforme le pid_t en string
    sprintf(pidStr, "%d", pidPere);

    if(pidFils == 0){
        //fils
        execl("./part1/jess","jess", "-p", pidStr, NULL);
    }
    //pere
    sprintf(pidStr, "%d", pidFils);
    execl("./part1/lavina","lavina", "-p", pidStr, NULL);
}