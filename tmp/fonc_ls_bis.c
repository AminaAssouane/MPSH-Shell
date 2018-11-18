#include "fonc_ls.h"


void fonction_ls_a(char * dir,int dep){
	DIR *dp;
	struct stat statbuf;
	struct dirent *entre;

	if((dp = opendir(dir)) == NULL){
		fprintf(stderr, "pas possible de ouverir ref: %s \n", dir);
	}

	chdir(dir);
	while((entre = readdir(dp)) != NULL){
		lstat(entre->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if(strcmp(".", entre->d_name)==0){
				printf("%*s%s\t",dep,"", entre->d_name);
				continue;
			}else if(strcmp("..", entre->d_name) == 0){
				printf("%*s%s\t",dep,"", entre->d_name);
				continue;
			}
			printf("%*s%s\t",dep,"", entre->d_name);
			
		}else{
			printf("%*s%s\t", dep,"",entre->d_name);
		}	
	}
	
	chdir("..");
	closedir(dp);
	
}


void fonction_ls(char * dir,int dep){
	DIR *dp;
	struct stat statbuf;
	struct dirent *entre;

	if((dp = opendir(dir)) == NULL){
		fprintf(stderr, "pas possible de ouverir ref: %s \n", dir);
	}

	chdir(dir);
	while((entre = readdir(dp)) != NULL){
		lstat(entre->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if(strcmp(".", entre->d_name)==0){
				printf("%*s%s\t",dep,"", entre->d_name);
				continue;
			}else if(strcmp("..", entre->d_name) == 0){
				printf("%*s%s\t",dep,"", entre->d_name);
				continue;
			}
			printf("%*s%s\t",dep,"", entre->d_name);
			
		}else{
			printf("%*s%s\t", dep,"",entre->d_name);
		}	
	}
	
	chdir("..");
	closedir(dp);
	
}
/*
int main(int argc, char* argv[]){

	char * topdir = ".";
	if(argc >=2){
		topdir = argv[1];
	}
	fonction_ls(topdir, 1);
}
*/
