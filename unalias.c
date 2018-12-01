#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


void unalias(char *comm){
  int fd = open("mpsh_aliases.txt",O_RDWR | O_CREAT);
  if (fd != -1){
    int fdnew = open("newal.txt",O_RDWR | O_CREAT);
    char newcom[50] = "alias ";
    char c;
    strcat(newcom,comm);
    int size = strlen(newcom);
    char *buff = malloc(size);
    int r;
    char bool = 0;
    while (read(fd,buff,size)>0){
      if (strncmp(newcom,buff,size) != 0){
    write(fdnew,buff,size);
	read(fd,&c,1);
	while ((c!='\n')&&(r > 0)){
	  write(fdnew,&c,1);
	  r = read(fd,&c,1);
	}
	if (r>0){
	  write(fdnew,&c,1);
	}
      }
      else {
	bool = 1;
	read(fd,&c,1);
	while (c!='\n'){
	  read(fd,&c,1);
	}
      }
    }
    // On supprime ensuite le fichier mpsh_aliases et on renomme le nouveau fichier
    chmod("mpsh_aliases.txt",S_IRWXU); 
    chmod("newal.txt",S_IRWXU);
    close(fd);
    close(fdnew);
    remove("mpsh_aliases.txt");
    rename("newal.txt","mpsh_aliases.txt");
    if (bool == 0){
      /*printf("\nAucun alias de ce nom.");*/
    }
    exit(-1);
  }
  else {
    printf("Erreur. Il n'y a aucun alias.");
    exit(-1);
  }
}

int main(int argc, char * argv[]){
  if (argc < 2){
    printf("\nTrop peu d'arguments pour la commande unalias.");
    return 1;
  }
  else {
    if (argc == 2){
      unalias(argv[1]);
      return 1;
    }
    else {
      printf("Trop d'arguments pour la commande unalias.");
      return 0;
    }
  }
}