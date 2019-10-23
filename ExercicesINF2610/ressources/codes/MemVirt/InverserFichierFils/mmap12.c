#include "../../../env_config/env_config.c"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include<sys/wait.h>
int main(int argc, char *argv[]) {
bootEnvVars(&argc, &argv);
char   *projection;
int    fd;
struct stat attr;
long   fsize;
int    i;
char   tmp;
if(argc != 2) {
   printf("Usage: ./a.out <nom_du_fichier>\n");
   exit(1);
}

fd = open(argv[1],O_RDWR);
if(fd==-1) exit(1);
stat(argv[1], &attr);

fsize = attr.st_size;
projection = (char *) mmap(NULL, fsize, PROT_READ | PROT_WRITE,
                           MAP_SHARED, fd, 0);
if(projection == (char *) MAP_FAILED) 		exit(1);
close(fd);
if(fork()==0)
//if(projection[fsize-1]=='\n')fsize--;
{  for(i=0; i<fsize/2; i++) {
   tmp = projection[i];
   projection[i] = projection[fsize - i -1];
   projection[fsize - i -1] = tmp;
}
munmap((void *) projection, fsize);
printf("Resultat: %s\n", projection);
printf("fini du fils \n");
return 0;
} 
wait(NULL);
for(i=0; i<fsize; i++) {
   
   write(1, &projection[i],1);}
return 0;
}


