#include  <stdio.h>
#include  <sys/types.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <sys/wait.h>
/*
 * Clone Lab - part1.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

void part1() {
    // TODO
    pid_t pidPere = getpid();
    pid_t pidFils = fork();

    char pidString[100];
    
    //char* pidStrpere = malloc(sizeof(pid_t));
    //char* pidStrfils = malloc(sizeof(pid_t));
    
    sprintf(pidString, "%d", pidPere);

    if(pidFils == 0){
        execl("./part1/andrew","andrew", "--p", pidString, NULL);
    }
    sprintf(pidString, "%d", pidFils);
   execl("./part1/contessa","contessa", "--p", pidString, NULL);
   // wait(NULL);
    

    
    
    
    
}