/*
 *------------Université Paris Diderot--------------------
 * Filename: mpsh.h
 * Version: 1.0 Beta
 * Created: 16h32m 02/11/18
 * Author: Equipe 33 (Yuchen BAI)
 *---------------------------------------------------------
 */

#include "mpsh.h"
#define TRUE 1
#define SHELL_BUFFER  64

char **read_input(char *input){
	
	char **cmd = malloc(8*sizeof(char *)); //pour l'instant, 8 character est maximum
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

int cd(char *path){
	return chdir(path);
}

void proc(){
	
	char **command;
	//char *input;
	pid_t child_pid;
	int stat_loc;
	
	char pathname[SHELL_BUFFER];

	while(TRUE){

		memset(pathname, 0, sizeof(pathname));
		getcwd(pathname, sizeof(pathname));		//obtenir l'adresse actuel
		printf("[%s]",pathname);
		fflush(stdout);					//renouvoler espace

		//input = readline("mpsh>> ");
		command = read_input(readline("~$ "));

		if(!command[0]){				//get_input ne marche pas, alors on arête
			//free(input);
			free(command);
			continue;
		}

		if(strcmp(command[0], "cd") == 0){
			if(cd(command[1])<0){
				perror(command[1]);
			}

			//à compléter

			continue;
		}

		child_pid = fork();

		if(child_pid <0){
			perror("Fork failed");
			exit(1);
		}

		if(child_pid == 0){
			if(execvp(command[0], command)<0) {
				perror(command[0]);
				exit(1);
			}
			printf("main : execvp failed \n");
		}else{
			waitpid(child_pid, &stat_loc, WUNTRACED);
		}

		free(command);
	}

}

int main(){

	proc();
	exit(0);
}
