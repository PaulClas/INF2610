/*
 * Comm Lab - puzzle1.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
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

#include "libcommlab.h"

void puzzle1() {
    // TODO

    const char* token = "d5dd1134e8f3";
    const char* wPipe = "/tmp/pipe_e2b5570d";

    const char* rPipe = "/tmp/pipe_05bbca44";

    mkfifo(rPipe, S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR);
    mkfifo(wPipe, S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR);

    pid_t cID = fork();
    if (cID == 0){
        execl("./puzzle1/exchanger", "exchanger",  NULL);
    }

    int fd = open(wPipe, O_WRONLY);
    if (fd != -1){ 
        write(fd, token, strlen(token));
        close(fd);
    }

    kill(cID, SIGUSR1); 
    
    char message[300];
    fd = open(rPipe, O_RDONLY);
    if(fd != -1) {
        while(read(fd, message, 100) > 0);
        printf("%s", message);
        close(fd);
    }

    checkExchangerMessage(message);

}