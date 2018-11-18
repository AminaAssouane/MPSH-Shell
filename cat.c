#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
void cat(char *path){
  struct stat statbuf;
  stat(path,&statbuf);;
  if (S_ISREG(statbuf.st_mode)){
    int fd = open(path,O_RDONLY);
    if (fd != -1){
      char c;
      int r = read(fd,&c, 1);
      while (r > 0){
    write(1, &c, 1);
	r = read(fd, &c, 1);
      }
      if (r < 0){
	printf("Erreur.");
	exit(-1);
      }
      close(fd);
    }
    else{
      printf("Le fichier n'existe pas.");
      exit(-1);
    }
  }
  else {
    printf("Erreur. Le chemin que vous avez mentionne n'est pas un fichier.");
    exit(-1);
  }
}

int main(int argc, char* argv[]){
  if (argc < 2){
    printf("Erreur.");
    return 0;
  }
  else {
    if (argc == 2){
      cat(argv[1]);
      return 1;
    }
    else { 
      printf("Erreur. Trop d'arguments pour la fonction cat.");
      return 0;
    }
  }
}
