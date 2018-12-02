/*
 *------------Université Paris Diderot--------------------
 * Filename: mpsh.h
 * Version: 1.0 Beta
 * Created: 16h32m 02/11/18
 * Author: Equipe 33 (Yuchen BAI)
 *---------------------------------------------------------
 */

#include "mpsh.h"


char **read_input(char *input){
    
	char **cmd = malloc(SHELL_BUFFER*sizeof(char *)); //pour l'instant, 8 character est maximum
	if(cmd == NULL){
		perror("get_input : malloc failed \n");
		exit(1);
	}

	char *sep = " "; //diviseur
	char *tmp;
	int index = 0;

	tmp = strtok(input, sep); //input est le premier variable, arête jusqu'à diviseur
	while (tmp != NULL){
		cmd[index] = tmp;
		index++;
		tmp = strtok(NULL, sep); //ici, NULL c'est le premier variable
	}

	cmd[index] = NULL;  //la fonction execvp demand le dernier character est NULL
	return cmd;
}

void make_prompt(){
	char pathname[SHELL_BUFFER];
	char username[SHELL_BUFFER];
	char hostname[SHELL_BUFFER];

	struct passwd * pwd = getpwuid(getuid());
	strcpy(username, pwd->pw_name);
	gethostname(hostname,SHELL_BUFFER);


	memset(pathname, 0, sizeof(pathname));
	getcwd(pathname, sizeof(pathname));		//obtenir l'adresse actuel
	printf("[%s-%s-%s]",username,hostname,pathname);
	fflush(stdout);					//renouvoler espace


}

int cd(char *path){
	return chdir(path);
}

char * pwd(){
	char res[SIZE];
	return getcwd(res,SIZE);
}

void my_umask (int argc, char **argv){
	if (argc == 2){
		if(strlen(argv[1])>3){
		printf("Droits impossible à changer (nombre incorrect)\n");
	}else{
		int n;
		n=atol(argv[1]);
		if (n>777)
			printf("Droits impossible à changer (nombre incorrect)\n");
		umask(n);
	}
	}else if (argc>2)
		printf("Trop d'arguments pour umask\n");
	else 
		printf("Manque l'argument\n");

}

void my_echo (int argv,char ** argc){
    int i;
    for (i=1;i<argv;i++){
    	printf("%s ",argc[i]);
    }
    printf("\n");
}

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

int my_unalias(int argc, char * argv[]){
  if (argc < 2){
    printf("Trop peu d'arguments pour la commande unalias.\n");
    return 1;
  }
  else {
    if (argc == 2){
      unalias(argv[1]);
      return 1;
    }
    else {
      printf("Trop d'arguments pour la commande unalias.\n");
      return 0;
    }
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

int my_alias(int argc, char ** argv){
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


int nbargs(char ** x){
	int i=0;
	while(x[i]!=NULL)
		i++;
	return i;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 2);
    strcpy(result, s1);
    strcat(result," ");
    strcat(result, s2);
    return result;
}

int history(int argc,char ** argv,char *h [],int nbcom){
	if(argc==1){
		int i;
		if(nbcom>1000){
			i=1000;
			while((i--)>0){
				printf("%s\n",h[nbcom-i]);
			}
			return 1;
		}else{
			for(i=0;i<nbcom;i++){
				printf("%s\n",h[i]);
			}
			return 1;
		}
	}else if (argc==2){
		if((isdigit(argv[1][0]))){
			int tmp = atoi(argv[1]);
			if(tmp>nbcom){
				printf("Trop grand nombre change en history %d\n",nbcom);
				for(int i =0;i<nbcom;i++)
					printf("%s\n",h[i]);
				return 1;
			}else {
				for (int i =tmp;i>0;i--)
					printf("%s\n",h[nbcom-i]);
				return 1;
			}
			
		}else{
			printf("Mauvais argument\n");
			return 0;
		}
	}
	return 1;
}
void proc(){

	int nbcom=0;
	char ** h= malloc(SIZE*sizeof(char*));
	int t=SIZE;
	char **command;
	int nbarg;
	//char *input;
	pid_t child_pid;
	int stat_loc;
	
	//char pathname[SHELL_BUFFER];

	while(TRUE){

		//char* input = readline("~$ ");
		make_prompt();
		command = read_input(readline("~s "));
		nbarg=nbargs(command);

		if(!command[0]){				//get_input ne marche pas, alors on arête
			free(command);
			continue;
		}
		nbcom++;
		if(nbcom==t){
			t*=2;
			h=realloc(h,t*sizeof(char *));
		}
		char s[SHELL_BUFFER];
		sprintf(s,"%d",nbcom);
		char * tmp=s;
		for (int i=0;i<nbarg;i++){
			tmp=concat(tmp,command[i]);
		}
		h[(nbcom-1)]=tmp;

		if(strcmp(command[0], "cd") == 0){
			if(cd(command[1])<0){
				perror(command[1]);
			}

			//à compléter
			 //test
		}else if (strcmp(command[0], "exit") == 0){
			exit(0);
		}else if(strcmp(command[0], "pwd") == 0){
			printf("%s\n", pwd());
		}else if(strcmp(command[0], "echo")==0){
			my_echo(nbarg,command);
		}else if(strcmp(command[0], "umask")==0){
			my_umask(nbarg,command);
		}else if(strcmp(command[0], "history")==0){
			history(nbarg,command,h,nbcom);
		}else if(strcmp(command[0],"alias")==0){
			my_alias(nbarg,command);
		}else if(strcmp(command[0],"unalias")==0){
			my_unalias(nbarg,command);
		}

		child_pid = fork();

		if(child_pid <0){
			perror("Fork failed");
			exit(1);
		}
		
		if(child_pid == 0){
			/*	
			if(execvp(command[0], command)<0) {
				perror(command[0]);
				exit(1);
			}*/
			if(strcmp(command[0], "ls")==0){
				fonctionls_main(nbarg,command);				
			}else if(strcmp(command[0],"cat")==0){
				if(nbarg>2){
					cat_n(nbarg,command);
				}else{
					cat(command[1]);
				}
			}else if (strcmp(command[0],"mkdir")==0){
				makeDir(command[1]);
			}
			exit(1);
			printf("main : execvp failed \n");
		}else{
			waitpid(child_pid, &stat_loc, WUNTRACED);
		}

	
		free(command);
	}
}

int main(int argc, char * argv[]){
	proc();
	exit(0);
}
