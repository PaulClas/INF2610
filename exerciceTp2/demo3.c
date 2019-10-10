/*
 * Démonstration 3:
 * On veut exécuter le programme ls dans un processus fils.
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    pid_t childPID = fork();
    if (childPID == 0) {
        char* params[] = {"ls", "-las", "/"};
        if (execvp("ls", params) < 0) {
            fprintf(stderr, "Erreur lors du exec\n");
            exit(1);
        }
    }
    wait(NULL);

    return 0;
}