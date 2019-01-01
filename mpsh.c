/*
 *------------Université Paris Diderot--------------------
 * Filename: mpsh.h
 * Version: 1.0 Beta
 * Created: 16h32m 02/11/18
 * Author: Equipe 33 (Amina ASSOUANE Mickeal Darmon Yuchen BAI )
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

void make_prompt1(){
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

void make_prompt(){
	char pathname[SHELL_BUFFER];
	char username[SHELL_BUFFER];
	char hostname[SHELL_BUFFER];

	struct passwd * pwd = getpwuid(getuid());
	strcpy(username, pwd->pw_name);
	gethostname(hostname,SHELL_BUFFER);
	memset(pathname, 0, sizeof(pathname));
	getcwd(pathname, sizeof(pathname));		//obtenir l'adresse actuel


	char res[1024];
	char *dest = malloc(sizeof(char)*1024);
	char filename[] = "mpshrc";
	FILE *fp;
	if((fp=fopen(filename,"r"))==NULL){
		printf("Erreur : make_prompt openfile! \n");
	}
	while(!feof(fp)){
		fgets(res,1024,fp);
		//printf("%s\n", res);
		if(strncmp(res,"INVITE='",8)==0){
			//printf("duitou\n");
			strcat(dest,res+8);
			break;
		}
	}

	int len = strlen(dest);
	dest[len-2] = '\0';
	//dest[len-1] = '\0';
	//printf("dest len: %d ",len);
	char tmp[1024];
	//int lent=0;
	for(int i=0;i<len-2;i++){
		if(dest[i]=='$'){ //des options de INVITE
			if(dest[i+1]=='H'){
				strcat(tmp,hostname);
				i++;
				
			}else if(dest[i+1] == 'P'){
				strcat(tmp,pathname);
				i++;
			
			}else if(dest[i+1] == 'U'){
				strcat(tmp,username);
				i++;
			}else{
				tmp[strlen(tmp)] = dest[i];

			}
			
		}else{
			//printf("%c \n",dest[i]);
			tmp[strlen(tmp)] = dest[i];
		}
	}	
	//printf("%s\n", tmp);
	tmp[strlen(tmp)] = '\0';
	dest = tmp+4;
	if(dest != NULL){
		printf("%s",dest);
		fflush(stdout);					//renouvoler espace
	}else{
		printf("[%s-%s-%s]~$",username,hostname,pathname);
		
		fflush(stdout);					//renouvoler espace
	}

}


int nbargs(char ** x){
	int i=0;
	while(x[i]!=NULL)
		i++;
	return i;
}

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 2);
    strcpy(result, s1);
    strcat(result," ");
    strcat(result, s2);
    return result;
}

char * commEntreparenthese(char * s){
	char *res= malloc(SHELL_BUFFER);
	int j=0;
	int i=0;
	while(s[i]!='(')
		i++;
	i++;
	while(s[i]!=')'){
		res[j]=s[i];
		i++;
		j++;
	}
	return res;
}

char * resteDeLaCommande(char **comm,int dep){
	int i;
	char * tmp= malloc(SIZE*sizeof(char));
	for(i=dep;i<nbargs(comm);i++)
		tmp=concat(tmp,comm[i]);
	return tmp;
}
//cd
int cd(char *path){
	return chdir(path);
}

//pwd
char * pwd(){
	char res[SIZE];
	return getcwd(res,SIZE);
}

//umask
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

//echo
void my_echo (int argv,char ** argc){
    int i;
    for (i=1;i<argv;i++){
    	printf("%s ",argc[i]);
    }
    printf("\n");
}

int history(int argc,char ** argv,char *h [],int nbcom){
	if(argc==1){
		int i;
		if(nbcom>1000){
			i=1000;
			while(i>=0){
				printf("%s\n",h[nbcom-i-1]);
				i--;
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
int egalecomm(char **comm){
	if(nbargs(comm)==1){
		int i;
		for (i=0;i<strlen(comm[0]);i++){
			if(comm[0][i]=='=')
				return 0;
		}
	}
	return -1;
}

int AliasComp(char *comm, char * fin, char** ali, int nbali){
	int i=0;
	int j=0;
	int k=0;
	char * tmp=malloc(SIZE*sizeof(char));
	while(k<nbali){	
		while(ali[k][i]!='='){
			tmp[i]=ali[k][i];
			i++;
		}
		i++;//Saut '='
		tmp[i]='\0';
		if(strcmp(tmp,comm)==0){
			while(i<strlen(ali[k])){
				fin[j]=ali[k][i];
				i++;
				j++;
			}
			fin[j]='\0';
			return 1;
		}
		i=0;
		j=0;
		tmp[i]='\0';
		fin[i]='\0';
		k++;
	}
	fin="Pas de alias trouver !";
	return -1;
}

//type
short type(char* command){
	char* cmdInterne[9] = {"cd", "exit", "pwd", "echo", "unmask", "history", "alias", "unalias","type"};
	for(int i=0;i<9;i++){
		if(strcmp(command,cmdInterne[i])==0){
			printf("%s est une commande primitive du shell\n",command);
			return 0;
		}
	}
	/*if(estAlias(command))
		return 1;*/
	return 2;
}


void aliasA(char ** e,int nbexp){
	int i;
	printf("%d\n",nbexp );
	for (i=0;i<nbexp;i++){
			printf("alias ");
			printf("%s\n",e[i]);
	}
}

int unalias(char** ali, int nbali,char* cmd){
	int len=strlen(cmd);
	for(int j=0;j<nbali;j++){
		if(strncmp(ali[j],cmd,len)==0){
			for (int i=j;i<nbali-1;i++){
				ali[i][0]='\0';
				memmove(ali[i],ali[i+1],strlen(ali[i+1]));
			}
			return 1 ;
		}
	}
	printf("%s n'est pas in commande alias\n", cmd);	
	return -1;
}

//export
void exportN(char ** e,int nbexp){
	int i;
	for (i=0;i<nbexp;i++){
		printf("%s\n",e[i]);
	}
}

int ajoutExp(char* s,char ** e,int nbexp){
	int i;
	int j=0;
	char *tmp=malloc(SIZE*sizeof(char));
	for (i=0;i<nbexp;i++){
		while(e[i][j]!='=' && e[i][j]!='\0'){
			tmp[j]=e[i][j];
			j++;
		}
		if(strncmp(s,tmp,j)==0)
			return i;
	}
	return -1;
}

char * parcoursexp(char *s,char ** e,int nbexp){
	int i=0;
	int j=0;
	int k=0;
	char * tmp=malloc(SIZE*sizeof(char));
	char * tmp2=malloc(SIZE*sizeof(char));
	while(k<nbexp){
		while(e[k][i]!='='){
			tmp[i]=e[k][i];
			i++;
		}
		i++;
		tmp[i]='\0';
		if(strcmp(tmp,s)==0){
			while(i<strlen(e[k])){
				tmp2[j]=e[k][i];
				i++;
				j++;
			}
			j++;
			tmp2[j]='\0';
			return tmp2;
		}
		i=0;
		j=0;
		tmp[i]='\0';
		tmp2[i]='\0';
		k++;
	}
	tmp2="Pas de export trouver !";
	return tmp2;
}

int erreur(char * err)
{
    write(2, err, strlen(err));
    return 1;
}


void parse(char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali){
	char * res=malloc(SIZE*sizeof(char));
	int nbarg=nbargs(command);
	int d=0;
	if(strcmp(command[0], "cd") == 0 && d==0){
			if(cd(command[1])<0){
				perror(command[1]);
			}
		}else if(command[0][0]=='$' && d==0){
			char * tmp=malloc(SIZE*sizeof(char));
			char * tmp2=malloc(SIZE*sizeof(char));			
			tmp=parcoursexp((command[0])+1,exp,nbexp);
			if(strcmp(tmp,"Pas de export trouve !")==1){
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
			}else{
				tmp=parcoursexp((command[0])+1,eg,nbeg);
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}
		}else if (egalecomm(command)==0 && d==0){
			int i=ajoutExp(command[0],eg,nbeg);
			if(i>=0){
					eg[i]=command[0];
					d++;
			}else if (i==-1){
				eg[nbeg]=command[0];
				nbeg++;
				d++;
			}else{
				printf("Ne peut pas être parser\n");
				d++;
			}
		}else if (strcmp(command[0], "exit") == 0 && d==0){
			exit(0);
			d++;
		}else if(strcmp(command[0], "pwd") == 0 && d==0){
			printf("%s\n", pwd());
			d++;
		}else if(strcmp(command[0], "echo")==0 && d==0){
			if(command[1][0]=='$'){
				char * tmp=malloc(SIZE*sizeof(char));
				tmp=concat(parcoursexp((command[1])+1,exp,nbexp),(resteDeLaCommande(command,2)));
				printf("%s\n", tmp);
				d++;
			}else{
				my_echo(nbarg,command);
				d++;
			}
		}else if(strcmp(command[0], "umask")==0 && d==0){
			my_umask(nbarg,command);
			d++;
		}else if(strcmp(command[0], "history")==0 && d==0){
			history(nbarg,command,h,nbcom);
			d++;
		}else if(strcmp(command[0], "helpme")==0){
			printf("can't help you !\n");
		}else if(strcmp(command[0],"alias")==0 && d==0){
			d++;
			if(nbarg==1){
				aliasA(ali,nbali);
			}else if (nbarg==2){
				int i = ajoutExp(command[1],ali,nbali);
				printf("%d\n",i);
				if(i>=0)
					ali[i]=command[1];
				else if (i==-1){
					ali[nbali]=command[1];
					nbali++;
				}					
			}else
				printf("Trop d'arguments pour alias\n");
		}
		else if(strcmp(command[0],"unalias")==0 && d==0){
			unalias(ali,nbali,command[1]);
			d++;
		}else if(strcmp(command[0],"type")==0 && d==0){
			int t = type(command[1]);
			if (t==2){
				printf("%s est /bin/%s\n",command[1],command[1]);
			}
			d++;
		}else if(strcmp(command[0],"export")==0 && d==0){
			d++;
			if(nbarg!=2){
				printf("Mauvais nombre d'argument\n");
			}else if (strcmp(command[1],"-n")==0){
				exportN(exp,nbexp);
			}else{
				int i = ajoutExp(command[1],exp,nbexp);
				if(i>=0)
					exp[i]=command[1];
				else if (i==-1){
					exp[nbexp]=command[1];
					nbexp++;
				}else
					printf("Ne peut pas être un export\n");					
			}
		}
		else if(AliasComp(command[0],res,ali,nbali)==1){ //ne marche pas maintenant
			char * tmp=res;
			for (int i=1;i<nbarg;i++){
				tmp=concat(tmp,command[i]);
			}
			free(command);
			command=read_input(tmp);
			parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
		}
	}

void addCurrentPathToRc(){

	char currentPath[80];
	getcwd(currentPath,sizeof(currentPath));


	const char *filename = "currentPath/abc";
	int p = open(filename, O_APPEND);
	
	char *res = "export PATH=\"";
	strcat(res,currentPath);
	char *tail = ":\$PATH";
	strcat(res,tail);

	write(p,res,strlen(res));
	close(p);
}

int redirectionS(char *dir,char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali)
{
	int t = type(command[0]);
	int nbarg=nbargs(command);
	pid_t fils=fork();
    if(fils==0){
   		int file = open(dir, O_CREAT|O_WRONLY, 0777);
    	if(file == -1)
    	{
        // En cas d'erreur
      	  erreur("Erreur : open\n");
    	}
 	
    // On redirige 1 vers file
    	dup2(file,1);
 
    	close(file);
    // On execute les commandes
    	if (t==0){
    		parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
    	}else if(strcmp(command[0], "ls")==0){
				fonctionls_main(nbarg,command);				
		}else if(strcmp(command[0],"cat")==0){
			if(nbarg>2){
				cat_n(nbarg,command);
			}else{
				cat(command[1]);
			}
		}else if (strcmp(command[0],"mkdir")==0){
			make_Dir(command[1]);
		}
    	close(1);
    	exit(1);
    }
    return 1;
}

int redirectionSerr(char *dir,char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali)
{
	int t = type(command[0]);
	int nbarg=nbargs(command);
	pid_t fils=fork();
    if(fils==0){
   		int file = open(dir, O_CREAT|O_WRONLY, 0777);
    	if(file == -1)
    	{
        // En cas d'erreur
      	  erreur("Erreur : open\n");
    	}
 	
    // On redirige 2 vers file
    	dup2(file,2);
 
    	close(file);
    // On execute les commandes
    	if (t==0){
    		parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
    	}else if(strcmp(command[0], "ls")==0){
				fonctionls_main(nbarg,command);				
		}else if(strcmp(command[0],"cat")==0){
			if(nbarg>2){
				cat_n(nbarg,command);
			}else{
				cat(command[1]);
			}
		}else if (strcmp(command[0],"mkdir")==0){
			make_Dir(command[1]);
		}
    	close(1);
    	exit(1);
    }
    return 1;
}


int alias(char ** ali,char * tmp){
	if (ali==NULL){
		return -1;
	}
	
	for(int i=0;i<SIZE;i++){
		if(ali[i]==NULL){
			ali[i] = tmp;
			return 1;
		}
		if (i==(SIZE-1)){
			printf("ali is full");
		}
	}

	return -1;
}

void show_alias(char ** ali){
	if(ali==NULL){
		printf("char ** ali est null");
	}

	int len = sizeof(ali)/sizeof(ali[0]);
	for(int i=0;i<len;i++){
		printf("%s\n",ali[i]);
	}
}

void proc(){
	int nbcom=0;
	char ** h= malloc(SIZE*sizeof(char*));
	int nbexp = 0;
	char ** exp = malloc(SIZE*sizeof(char*));
	int nbeg = 0; //Du type alias=commande
	char ** eg = malloc(SIZE*sizeof(char*));
	int nbali=0;
	int t=SIZE;
	char **command;
	int nbarg;
	pid_t child_pid;
	int stat_loc;
	char res[SHELL_BUFFER];
	char * vague = malloc(SIZE*sizeof(char));
	char * home = "/home/";
	strcpy(vague,home);
	strcat(vague,getenv("USER"));
	
	char ** ali = malloc(SIZE*sizeof(char*));
	if(ali==NULL){
		printf("ali est dans le proc nulli avant while");
	}	

	while(TRUE){
		int d=0;
		make_prompt();
		command = read_input(readline(" "));
		nbarg=nbargs(command);

		for (int cmp=0;cmp<nbarg;cmp++){
			if (command[cmp][0]=='~'){
				char * s=malloc(SIZE*sizeof(char));
				strcpy(s,vague);
				for(int i=1;i<strlen(command[cmp]);i++){	
					s[strlen(vague)+(i-1)]=command[cmp][i];
				}
				s[strlen(vague)+strlen(command[cmp])-1]='\0';
				command[cmp]=s;
			}
		}

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


		for (int cmp=1;cmp<nbarg;cmp++){
			if(command[cmp][0]=='>'){
				char * tmp = malloc(SIZE*sizeof(char));
				char * dir = command[cmp+1];
				for(int cmp2=0;cmp2<nbarg;cmp2++){
					if(strcmp(command[cmp2],">")!=0){
						if(cmp2==0)
							strcpy(tmp,command[cmp2]);
						else
							tmp=concat(tmp,command[cmp2]);
					}else 
						break;

				}
				free(command);
				command=read_input(tmp);
				redirectionS(dir,command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}
		}

		/*for(int i = 0;i<nbarg;i++){
			if(strcmp(command[i],"2>")){
				char * tmp = malloc(SIZE*sizeof(char));
				char * dir = command[i+1];
				for(int cmp2=0;cmp2<nbarg;cmp2++){
					if(strcmp(command[cmp2],">")!=0){
						if(cmp2==0)
							strcpy(tmp,command[cmp2]);
						else
							tmp=concat(tmp,command[cmp2]);
					}else 
						break;
				}
				free(command);
				command=read_input(tmp);
				redirectionSerr(dir,command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}
		}*/

		if(strcmp(command[0], "cd") == 0 && d==0){
			if(cd(command[1])<0){
				perror(command[1]);
			}
		}else if(command[0][0]=='$' && d==0){
			char * tmp=malloc(SIZE*sizeof(char));
			char * tmp2=malloc(SIZE*sizeof(char));			
			tmp=parcoursexp((command[0])+1,exp,nbexp);
			if(strcmp(tmp,"Pas de export trouve !")!=0){
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
			}else{
				tmp=parcoursexp((command[0])+1,eg,nbeg);
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}
		}else if (egalecomm(command)==0 && d==0){
			int i=ajoutExp(command[0],eg,nbeg);
			if(i>=0){
					eg[i]=command[0];
					d++;
			}else if (i==-1){
				eg[nbeg]=command[0];
				nbeg++;
				d++;
			}else{
				printf("Ne peut pas être parser\n");
				d++;
			}
		}else if (strcmp(command[0], "exit") == 0 && d==0){
			exit(0);
			d++;
		}else if(strcmp(command[0], "pwd") == 0 && d==0){
			printf("%s\n", pwd());
			d++;
		}else if(strcmp(command[0], "echo")==0 && d==0){
			if(command[1][0]=='$'){
				char * tmp=malloc(SIZE*sizeof(char));
				tmp=concat(parcoursexp((command[1])+1,exp,nbexp),(resteDeLaCommande(command,2)));
				printf("%s\n", tmp);
				d++;
			}else{
				my_echo(nbarg,command);
				d++;
			}
		}else if(strcmp(command[0], "umask")==0 && d==0){
			my_umask(nbarg,command);
			d++;
		}else if(strcmp(command[0], "history")==0 && d==0){
			history(nbarg,command,h,nbcom);
			d++;
		}else if(strcmp(command[0], "helpme")==0){
			printf("can't help you !\n");
		}else if(strcmp(command[0],"alias")==0 && d==0){
			d++;
			if(nbarg==1){
				aliasA(ali,nbali);
			}else if (nbarg==2){
				int i = ajoutExp(command[1],ali,nbali);
				printf("%d\n",i);
				if(i>=0)
					ali[i]=command[1];
				else if (i==-1){
					ali[nbali]=command[1];
					nbali++;
				}					
			}else
				printf("Trop d'arguments pour alias\n");
		}
		else if(strcmp(command[0],"unalias")==0 && d==0){
			unalias(ali,nbali,command[1]);
			nbali--;
			d++;
		}else if(strcmp(command[0],"type")==0 && d==0){
			int t = type(command[1]);
			if (t==2){
				printf("%s est /bin/%s\n",command[1],command[1]);
			}
			d++;
		}else if(strcmp(command[0],"export")==0 && d==0){
			d++;
			if(nbarg!=2){
				printf("Mauvais nombre d'argument\n");
			}else if (strcmp(command[1],"-n")==0){
				exportN(exp,nbexp);
			}else{
				int i = ajoutExp(command[1],exp,nbexp);
				if(i>=0)
					exp[i]=command[1];
				else if (i==-1){
					exp[nbexp]=command[1];
					nbexp++;
				}else
					printf("Ne peut pas être un export\n");					
			}
		}
		else if(AliasComp(command[0],res,ali,nbali)==1){ //ne marche pas maintenant
			char * tmp=res;
			for (int i=1;i<nbarg;i++){
				tmp=concat(tmp,command[i]);
			}
			free(command);
			command=read_input(tmp);
			parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
		}

		child_pid = fork();

		if(child_pid <0){
			perror("Fork failed");
			exit(1);
		}
		
		if(child_pid == 0){
			if(strcmp(command[0], "ls")==0 && d==0){
				d++;
				fonctionls_main(nbarg,command);				
			}else if(strcmp(command[0],"cat")==0 && d==0){
				d++;
				if(nbarg>2){
					cat_n(nbarg,command);
				}else{
					cat(command[1]);
				}
			}else if (strcmp(command[0],"mkdir")==0 && d==0){
				d++;
				if(nbarg == 2){
					make_Dir(command[1]);
				}else if(nbarg == 3 && strcmp(command[1],"-p")==0){
					printf("plusiers dirs");
					make_Dir(command[2]);
					//make_plu_Dirs(command[2]);
				}
			}
			exit(1);
			printf("main : execvp failed \n");
		}else{
			waitpid(child_pid, &stat_loc, WUNTRACED);
		}
		if(d==0)
			erreur("Erreur: commande inconnue\n");
		d=0;
		free(command);
	}
}

int main(int argc, char * argv[]){
	proc();
	exit(0);
}
