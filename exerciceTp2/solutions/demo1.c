/*
 * Démonstration 1:
 * Nous allons créer un programme qui crée deux processus fils.
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    printf("Hello from root process\n");

    pid_t child1PID = fork();
    if (child1PID == 0) {
        // Je suis le fils 1
        printf("Hello from child 1\n");
        exit(0);
    }

    pid_t child2PID = fork();
    if (child2PID == 0) {
        // Je suis le fils 2
        printf("Hello from child 2\n");
        exit(0);
    }

    while (wait(NULL) > 0);

    return 0;
}