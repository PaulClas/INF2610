/*
 * Comm Lab - puzzle1.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
// -------------------------------------------------

// #include "libcommlab.h"

// void puzzle1()
// {
//     char* message;
//     sprintf(message, "bonjour du processus %d", getpid());

// }

int main(){
    int fd;
    char message[100];
    sprintf(message, "bonjour du processus %d", getpid());
    printf("voici le message: %s \n", message);
    fd = open("mypipe", O_WRONLY);
    if(fd!=-1){
        write(fd, message, strlen(message)+1);
        close(fd);
    }else
        printf("desole, le tube n'est pas disponible");
    return 0;
}

    // const char *const token = "438c94025b31";
    // const char *const wPipe = "/tmp/pipe_e5deac00";
    // const char *const rPipe = "/tmp/pipe_2c149d4c";

    // // Effacer les fichiers de pipe s'ils existaient deja
    // remove(rPipe);
    // remove(wPipe);

    // mkfifo(rPipe, S_IRUSR | S_IWUSR);
    // mkfifo(wPipe, S_IRUSR | S_IWUSR);

    // pid_t cID = fork();
    // if (cID == 0)
    // {
    //     execl("./puzzle1/exchanger", "exchanger", NULL);
    // }

    // int fd = open(wPipe, O_WRONLY);
    // if (fd != -1)
    // {
    //     write(fd, token, strlen(token));
    //     close(fd);
    // }

    // kill(cID, SIGUSR1); 

    // char message[100];
    // fd = open(rPipe, O_RDONLY);
    // if(fd != -1) {
    //     while(read(fd, message, 100) > 0);
    //     printf("%s", message);
    //     close(fd);
    // }

    // checkExchangerMessage(message);