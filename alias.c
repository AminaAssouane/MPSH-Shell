#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cat.h"

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
    printf("Erreur. Il n'y a aucun alias.\n");
    exit(-1);
  }
}

/* fonction qui verifie si l'argument apres la commande alias est bien formaté */
short isAlias(char *comm, char *unalias){ 
  int i = 0;
  int size = strlen(comm); 
  if (comm[0] == '='){
    return 0;
  }
  while((comm[i] != '=') && (i < size)){
    unalias[i] = comm[i];
    i++;
  }
  if (comm[i] == '='){
    unalias[i] = '\0';
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
    char unali[100];
    if (isAlias(comm,unali) == 1){ // On vérifie si l'alias est bien formaté
      unalias(unali);
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
