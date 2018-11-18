#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


/* fonction qui verifie si l'argument apres la commande alias est bien formaté */
short isAlias(char *comm){ 
  int i = 0;
  int size = strlen(comm); 
  if (comm[0] == '='){
    return 0;
  }
  while((comm[i] != '=') && (i < size)){
    i++;
  }
  if (comm[i] == '='){
    return 1;
  }
  else {
    return 0;
  }
}

void alias(char* comm){
  if (comm == NULL){
    int fd2 = open("mpsh_aliases.txt", O_RDONLY | O_CREAT);
    close(fd2);
    cat("mpsh_aliases.txt"); // on appelle la fonction cat pour afficher le fichier qui contient les alias
  }
  else {
    if (isAlias(comm) == 1){ // On vérifie si l'alias est bien formaté
      int fd = open("mpsh_aliases.txt",O_RDWR | O_CREAT | O_APPEND);
      char newc[100] = "alias ";
      strcat(comm,"\n");
      strcat(newc,comm);
      write(fd,newc,strlen(comm)+6);
      close(fd);
    }
    else {
      printf("Erreur : alias : %s non trouve",comm);
    }
  }
}

int main(int argc, char * argv[]){
  if (argc < 2){
    alias(NULL);
    return 1;
  }
  else {
    if (argc == 2){
      alias(argv[1]);
      return 1;
    }
    else {
      printf("Erreur. Trop d'arguments pour la commande alias.");
      return 0;
    }
  }
}
