/*
 * Clone Lab - part2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include "libclonelab.h"
#include  <stdio.h>


// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include  <sys/types.h>
#include  <unistd.h>
#include  <sys/wait.h>
#include  <stdlib.h>
// -------------------------------------------------

#define PART2_OUTPUT_FILE_PATH "part2Output.txt"

void registerProcCaller(int level, int number){
    registerProc(level,number,getpid(), getppid());
}

void part2() {
    // Ouverture du fichier de sortie pour la question 2.3
    FILE* part2OutputFile = fopen(PART2_OUTPUT_FILE_PATH, "a");
    char* pidPere = malloc(sizeof(pid_t));// afin de transforme le pid_t en string
    sprintf(pidPere, "%d", getpid());
    fprintf(part2OutputFile,"Root process has pid %s", pidPere);
    
    if(fork() == 0){
        registerProcCaller(1, 1);
        if(fork() == 0){
            registerProcCaller(2,1);
            execl("./part2/level2.1","level2.1", NULL);
        }
        wait(NULL);
        if(fork() == 0){
            registerProcCaller(2,2);
            fprintf(part2OutputFile, " (message from process level2.2)\n");
            fclose(part2OutputFile);
            execl("./part2/level2.1","level2.2", NULL);
        }
        wait(NULL);
        fprintf(part2OutputFile, " (message from process level1.1)\n");
        fclose(part2OutputFile);
        execl("./part2/level1.1","level1.1", pidPere, NULL);
    }

    wait(NULL);

    if(fork() == 0){
        registerProcCaller(1, 2);
        if(fork() == 0){
            registerProcCaller(2,3);
            execl("./part2/level2.3","level2.3", NULL);
        }
        wait(NULL);
        fprintf(part2OutputFile, " (message from process level1.2)\n");
        fclose(part2OutputFile);
        execl("./part2/level1.2","level1.2", pidPere, NULL);
    }

    wait(NULL);

    if(fork() == 0){
        registerProcCaller(1, 3);
        if(fork() == 0){
            registerProcCaller(2,4);
            fprintf(part2OutputFile, " (message from process level2.4)\n");
            fclose(part2OutputFile);
            execl("./part2/level2.4","level2.4", NULL);
        }
        wait(NULL);
        execl("./part2/level1.3","level1.3", pidPere, NULL);
    }
    wait(NULL);

    if(fork() == 0){
        registerProcCaller(1, 4);
        if(fork() == 0){
            registerProcCaller(2,5);
            fprintf(part2OutputFile, " (message from process level2.5)\n");
            fclose(part2OutputFile);
            execl("./part2/level2.5","level2.5", NULL);
        }
        wait(NULL);
        execl("./part2/level1.4","level1.4", pidPere, NULL);
    }
    wait(NULL);
    fprintf(part2OutputFile, " (message from process level0)\n");
    fclose(part2OutputFile);
    execl("./part2/level0","level0", "630e831c0899cb57c8d76f60", NULL);
    
}