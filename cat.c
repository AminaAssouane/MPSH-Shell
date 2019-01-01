#include "cat.h"
 
int cat(char *path){
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
	return-1;
      }
      close(fd);
    }
    else{
      printf("Le fichier n'existe pas.");
      exit(-1);
      return -1;
    }
  }
  else {
    printf("Erreur. Le chemin que vous avez mentionne n'est pas un fichier.");
    exit(-1);
    return -1;
  }
  printf("\n");
  return 1;
}


int copy(FILE *fsrc, FILE *fdst){
	if(fsrc == NULL){
		perror("failed open fsrc");
		return 0;
	}
	if(fdst == NULL){
		perror("failed open fdst");
		return 0;
	}
	char c;
	while((c=fgetc(fsrc)) != EOF){
		fputc(c,fdst);
	}
	fclose(fsrc);
	fclose(fdst);

	return 1;
}

int nblines(FILE *f){
	if(f == NULL){
		perror("failed open f");
	}
	int n=0;
	char c;
	while((c=fgetc(f))!= EOF ){
		if(c=='\n'){
			n++;
		}
	}

	return n;
}

int lgEntier(int n){
	int cp=0;
	while((n/10) != 0){
		n=n/10;
		cp++;
	}
	return cp;
}

int cat_n(int argc, char* argv[]){

	const char *filename = argv[2];
	printf("%s\n", filename);

	FILE *p = fopen(filename, "rt+");
	int i = nblines(p);
	rewind(p);
	int l = lgEntier(i) + 1;
	char c;
	for(int n=0;n<i;n++){
		fprintf(stdout,"%*d",l,n);
		printf("  ");
		while((c=fgetc(p)) != '\n'){
			fputc(c,stdout);
		}
		printf("\n");	
	}	
	return 1;
}