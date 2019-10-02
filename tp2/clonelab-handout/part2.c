/*
 * Clone Lab - part2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include "libclonelab.h"

#include  <stdio.h>
#include  <sys/types.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <sys/wait.h>

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

#define PART2_OUTPUT_FILE_PATH "part2Output.txt"

void registerProcFunction(int level, int number){
            registerProc(level, number, getpid(), getppid());
    }


void part2() {
    //2.1
    //char* pidPere = malloc(sizeof(pid_t));
    char pidPere[100];
    FILE *part2OutputFile = fopen(PART2_OUTPUT_FILE_PATH, "w+");
    sprintf(pidPere, "%d", getpid());
    fprintf(part2OutputFile,"Root process has pid %s", pidPere);
    //fprintf(part2OutputFile,"(message from process level0)\n");
    //fclose(part2OutputFile); 
    if(!fork()){//1.1
        registerProcFunction(1, 1);
        
        if(fork()==0){//2.1
            registerProcFunction(2,1);
            fprintf(part2OutputFile," (message from process level2.1)\n");
            fclose(part2OutputFile);
            execl("./part2/level2.1","level2.1", NULL);
            
        }
       
        
        wait(NULL);
        execl("./part2/level1.1","level1.1",pidPere, NULL);
    }
    wait(NULL);

    if(fork()==0 ) { //1.2
        registerProcFunction(1, 2);
        //fprintf(part2OutputFile,"(message from process level1.2)\n");
        //fclose(part2OutputFile);
        if(fork()==0){ //2.2
            registerProcFunction(2, 2);
            //fprintf(part2OutputFile,"(message from process level2.2)\n");
            //fclose(part2OutputFile);
            execl("./part2/level2.2","level2.2", NULL);
        }
        wait(NULL);
        if(fork()==0){ //2.3
            registerProcFunction(2, 3);
            fprintf(part2OutputFile," (message from process level2.3)\n");
            fclose(part2OutputFile);
            execl("./part2/level2.3","level2.3", NULL);
        }
        
        wait(NULL);
        fprintf(part2OutputFile," (message from process level1.2)\n");
        fclose(part2OutputFile);
        execl("./part2/level1.2","level1.2",pidPere, NULL);
    }
    wait(NULL);

    if(fork()==0){ //1.3
        registerProcFunction(1, 3);
        fprintf(part2OutputFile," (message from process level1.3)\n");
        fclose(part2OutputFile);
        if(fork()==0){ //2.4
            char* env[] = {"NO_CHILD_TOKEN=72a8940af6722f7799dc509a", NULL};
            registerProcFunction(2, 4);
            execle("./part2/level2.4","level2.4", NULL, env);
        }
        
        wait(NULL);
        execl("./part2/level1.3","level1.3",pidPere, NULL);
    }
    wait(NULL);
 //wait(NULL);
    if(fork()==0){ // 1.4
        registerProcFunction(1, 4);
        if(fork()==0){ //2.5
            registerProcFunction(2, 5);
            
            fprintf(part2OutputFile," (message from process level2.5)\n");
            fclose(part2OutputFile);   

            execl("./part2/level2.5","level2.5", NULL);
        }
        wait(NULL);
        execl("./part2/level1.4","level1.4",pidPere, NULL);
    }
    wait(NULL);
    fprintf(part2OutputFile," (message from process level0)\n");
    fclose(part2OutputFile);   
    execl("./part2/level0","level0","ca5b2d134efd0252a9e8ad5e", NULL);
 


    
}