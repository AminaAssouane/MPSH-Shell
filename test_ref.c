#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

int test_ref(char * ref){

	struct stat but;
	if(stat(ref,&but) == 0){
		printf("oui\t");
		printf("numéro d'inoeud %d \n", but.st_ino);

		char* s; 
		if(S_ISREG(but.st_mode)){
			s = "fichier ordinaire";
		}else if(S_ISDIR(but.st_mode)){
			s = "répertoire";
		}else if(S_ISBLK(but.st_mode)){
			s = "lien sympolique";
		}else if(S_ISFIFO(but.st_mode)){
			s = "FIFO fichiers";
		}else if(S_ISLNK(but.st_mode)){
			s = "sympole fichier";
		}else if(S_ISCHR(but.st_mode)){
			s = "character file";
		}else{
			s = "unknown mode";
		}
		printf("type de fichier : %s \n", s);

	}else{
		printf("none");
		return 0;
	}
	if(but.st_mode & S_IXUSR){
		printf("fichier est exécutable\n");
	}else{
		printf("fichier est  non exécutable\n");

	}

	return 1;
}

int main(){

	char * ref;
	ref = "/home/yuchen/Documents/L3_INFO_SYS/TP5/a.out";

	int t = test_ref(ref);
	exit(0);
}
