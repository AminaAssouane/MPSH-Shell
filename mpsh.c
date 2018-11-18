/*
 *------------Université Paris Diderot--------------------
 * Filename: mpsh.h
 * Version: 1.0 Beta
 * Created: 16h32m 02/11/18
 * Author: Equipe 33 (Yuchen BAI)
 *---------------------------------------------------------
 */

#include "mpsh.h"
#include "new_ls2.h"
#define TRUE 1
#define SHELL_BUFFER  64
int main_ls(int argc, char** argv);

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

void proc(){	
	char **command;
	pid_t child_pid;
	int stat_loc;
	
	while(TRUE){

		//char* input = readline("~$ ");
		make_prompt();
		command = read_input(readline("~s "));
		int lg;
		lg = strlen(*command);

		if(!command[0]){				//get_input ne marche pas, alors on arête
			free(command);
			continue;
		}

		if(strcmp(command[0], "cd") == 0){
			if(cd(command[1])<0){
				perror(command[1]);
			}

			//à compléter

			continue; //test
		}
		else if (strcmp(command[0], "exit") == 0){
			
			exit(0);
		}else if(strcmp(command[0], "mpsh") == 0){
			printf("commands: \n");
			printf("ls : xxxx\n");
			continue;
		}

		child_pid = fork();

		if(child_pid <0){
			perror("Fork failed");
			exit(1);
		}

		if(child_pid == 0){
			printf("command : %s\n", command[1]);
			if(strcmp(command[0], "ls") == 0){	
				main_ls(lg, *command);
				/*	
				if(strcmp(command[1], "-l") == 0){
					printf("wait\n");
				}
				*/
				exit(1);
			}

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
