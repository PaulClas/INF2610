/*
 * Comm Lab - puzzle3.c
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
#include <sys/wait.h>
// -------------------------------------------------

void puzzle3()
{

    int fd[2];
    if (pipe(fd) != -1)
    {
        if (fork() == 0)
        {
            int file = open("./puzzle3/tmpfile", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
            close(fd[0]);
            close(fd[1]);
            dup2(file, STDOUT_FILENO);
            dup2(file, STDERR_FILENO);
            close(file);
            execl("./puzzle3/exc1", "exc1", NULL);
        }
        wait(NULL);

        if (fork() == 0)
        {
            close(fd[0]);
            dup2(fd[1], STDERR_FILENO);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            execl("./puzzle3/exc2", "exc2", NULL);
        }

        if (fork() == 0)
        {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            dup2(STDOUT_FILENO, STDERR_FILENO);
            close(fd[0]);
            execl("./puzzle3/exc3", "exc3", NULL);
        }

        wait(NULL);
        wait(NULL);

        if (fork() == 0)
        {
            int file = open("./puzzle3/tmpfile", O_CREAT | O_RDONLY | O_WRONLY, S_IRUSR | S_IWUSR);
            close(fd[1]);
            close(fd[0]);
            dup2(file, 
            );
            close(file);
            execl("./puzzle3/exc4", "exc4", NULL);
        }

        wait(NULL);

        close(fd[0]);
        close(fd[1]);
    }
}