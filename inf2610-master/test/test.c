//programme	testpipe5.c
#include <sys/wait.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int fd[2];
    pid_t cpid;
    char buf;
    assert(argc == 2);
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    } //	EXIT_FAILURE	=	1
    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {                 /*	Child	reads from	pipe	*/
        close(fd[1]); /*	Close	unused write	end	*/
        while (read(fd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1); //	write(1,	&buf,	1);
        printf("le processus %d vient", getpid());
        write(STDOUT_FILENO, "\n", 1);
        close(fd[0]); 
        exit(EXIT_SUCCESS); //	EXIT_SUCCESS	=	0
    }
    else
    {                 /*	Parent	writes argv[1]	to	pipe	*/
        close(fd[0]); /*	Close	unused read	end	*/
        write(fd[1], argv[1], strlen(argv[1]));
        close(fd[1]); /*	Reader	will see	EOF	*/
        wait(NULL);   /*	Wait	for	child	*/
        exit(EXIT_SUCCESS);
    }
}