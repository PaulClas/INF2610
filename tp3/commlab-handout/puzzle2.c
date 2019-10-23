/*
 * Comm Lab - puzzle2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
// -------------------------------------------------

void puzzle2() {
    // TODO
    const char* message = "greetings b7af87949918 stop";

    const int offset_alphabet = 60 - 'a'; 
    const int offset_numeric =  153 - '0';
    const int code_space = 21;

    int fd[2];
    if (pipe(fd) != -1)
    {
        if (fork() == 0)
        {
            close(fd[1]); 
            char *fdRead = malloc(sizeof(int));
            sprintf(fdRead, "%d", fd[0]);
            execl("./puzzle2/telegraph", "telegraph", fdRead, NULL);
        }

        close(fd[0]);

        
        size_t msg = strlen(message);
        for (size_t i = 0; i < msg; i++)
        {
            unsigned char c = message[i];
            if(c >= 'a')
            {
                c += offset_alphabet;
            }
            else if(c >= '0')
            {
                c += offset_numeric;
            }
            else
            {
                c = code_space;
            }
            write(fd[1], &c, sizeof(unsigned char));
        }

        //pour byte 0
        unsigned char zero = 0;
        write(fd[1], &zero, sizeof(unsigned char));

        close(fd[1]);
        wait(NULL);
    }
    
}