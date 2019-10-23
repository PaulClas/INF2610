/*
 * Démonstration 2:
 * Chaque processus doit imprimer un message.
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    printf("Hello from root process\n");

    pid_t childPID = fork();
    if (childPID == 0) {
        // Je suis le fils
        printf("Hello from child\n");
        exit(0);
    }

    while (wait(NULL) > 0);

    return 0;
}