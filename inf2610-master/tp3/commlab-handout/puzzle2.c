/*
 * Comm Lab - puzzle2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
// -------------------------------------------------

void puzzle2()
{
    const char *const message = "here is token 2b0b49a0be58 done";

    // Difference entre les codes ASCII et les codes du telegraph
    const int offset_alphabet = 62 - 'a'; 
    const int offset_numeric = 218 - '0';
    const int code_space = 7;

    int fd[2];
    if (pipe(fd) != -1)
    {
        if (fork() == 0)
        {
            close(fd[1]); // Fermer le descripteur d'ecriture pour le tube du telegraph
            char *fdRead = malloc(sizeof(int));
            sprintf(fdRead, "%d", fd[0]);
            execl("./puzzle2/telegraph", "telegraph", fdRead, NULL);
        }

        close(fd[0]);

        // Effectuer la traduction entre le code ASCII et le code du telegraph et transmettre le
        size_t msgLen = strlen(message);
        for (size_t i = 0; i < msgLen; i++)
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

        // Ecrire le caractere qui termine le message
        unsigned char zero = 0;
        write(fd[1], &zero, sizeof(unsigned char));

        close(fd[1]);
        wait(NULL);
    }
}