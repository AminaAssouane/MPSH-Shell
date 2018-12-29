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

//Ce qui provoque l'effacement dans le fichier
void unalias(char *comm,char* alias_path1, char* alias_path2){
	//int fd = open("mpsh_aliases.txt",O_RDWR | O_CREAT);
  	int fd = open(alias_path1,O_RDWR | O_CREAT);

	if (fd != -1){
   		//int fdnew = open("newal.txt",O_RDWR | O_CREAT);
		int fdnew = open(alias_path2,O_RDWR | O_CREAT);

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
	    		r = read(fd,&c,1);
	    		while ((c!='\n')&&(r > 0)){
	        		write(fdnew,&c,1);
	        		r = read(fd,&c,1);
	    		}
	    	if (r>0){
	        	write(fdnew,&c,1);
	    	}
      	} else {
			bool = 1;
			read(fd,&c,1);
			while (c!='\n'){
	  			read(fd,&c,1);
			}
      	}
    }
	// On supprime ensuite le fichier mpsh_aliases et on renomme le nouveau fichier
    //chmod("mpsh_aliases.txt",S_IRWXU); 
    //chmod("newal.txt",S_IRWXU);
    chmod(alias_path1,S_IRWXU); 
    chmod(alias_path2,S_IRWXU);
    close(fd);
    close(fdnew);
    //remove("mpsh_aliases.txt");
    //rename("newal.txt","mpsh_aliases.txt");
    remove(alias_path1);
    rename("newal","mpshrc");
    if (bool == 0){
      /*printf("\nAucun alias de ce nom.");*/
    }
	} else {
    	printf("Erreur. Il n'y a aucun alias.\n");
  	}
}

//Main de unalias
int my_unalias(int argc, char * argv[], char* alias_path1,char* alias_path2){
  	if (argc < 2){
   		printf("Trop peu d'arguments pour la commande unalias.\n");
    	return 1;
  	} else {
    	if (argc == 2){
      		unalias(argv[1],alias_path1,alias_path2);
      		return 1;
    	} else {
      		printf("Trop d'arguments pour la commande unalias.\n");
      		return 0;
    	}
  	}
}

/* Fonction qui verifie si l'argument apres la commande alias est bien formaté */
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
  	} else {
    	return 0;
  	}
}

void alias(char* comm,char* alias_path1,char* alias_path2){
	if (comm == NULL){
    	//int fd = open("mpsh_aliases.txt", O_RDONLY | O_CREAT);
    	int fd = open(alias_path1, O_RDONLY | O_CREAT);

	close(fd);
	cat(alias_path1);
    	//cat("mpsh_aliases.txt"); // on appelle la fonction cat pour afficher le fichier qui contient les alias
  	} else {
    	char unali[100];
    	if (isAlias(comm,unali) == 1){ // On vérifie si l'alias est bien formaté
     		unalias(unali,alias_path1,alias_path2);
      		//int alias = open("mpsh_aliases.txt",O_RDWR | O_CREAT | O_APPEND);
      		int alias = open(alias_path1,O_RDWR | O_CREAT | O_APPEND);

		char newc[100] = "alias ";
      		strcat(comm,"\n");
      		strcat(newc,comm);
      		printf("newc = %s\n", newc);
      		write(alias,newc,strlen(newc));
      		close(alias);
    	} else {
      		printf("Erreur : alias : %s non trouve",comm);
    	}
  	}
}

int my_alias(int argc, char ** argv, char* alias_path1,char* alias_path2){
	if (argc < 2){
    	alias(NULL,alias_path1,alias_path2);
    	return 1;
  	} else {
    	if (argc == 2){
      		alias(argv[1],alias_path1,alias_path2);
      		return 1;
    	} else {
      		printf("Erreur. Trop d'arguments pour la commande alias.");
     	 	return 0;
   		}
  	}
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

char * path_alias_txt(char* filename){	
	char filePath[256];
	getcwd(filePath,sizeof(filePath));
	strcat(filePath,filename);
	printf("%s\n", filePath);

	return filePath;
}

int estAlias(char *comm,char* alias_path1,char* alias_path2){
	//int f = open("mpsh_aliases.txt", O_RDONLY );
	int f = open(alias_path1, O_RDONLY );

	//if(f==-1)
		//printf("impossible d'ouvrir le fichier\n");
	int i=0,j=0,k=0;
	char c=read(f,&c,1);
	char prec;
	char * s=malloc(SHELL_BUFFER*sizeof(char)); 
	char * res=malloc(SHELL_BUFFER*sizeof(char));
	while(c!=EOF){//Lors de ma boucle je ne trouve pas '\0', aucune idée de pourquoi
		while (c!='\n'){
	  		s[i]=c;
	  		i++;
	  		read(f,&c,1);
		}
		s[i]='\0';
		while(s[j]!=' ')
			j++;
		j++;//Saut de l'espace
		while(s[j]!='='){
			res[k]=s[j];
			k++;
			j++;
		}
		j++;//Saut de '='
		if(strcmp(comm,res)==0){
			printf("%s est un alias de %s \n",res,s+j);
			return 1;
			break;
		}
		prec=c;
		read(f,&c,1);
		if(prec==c)
			return 0;
		i=0;
		j=0;
		k=0;
	}
	close(f);
	return 0;
}

int AliasComp(char *comm, char * fin, char* alias_path1, char* alias_path2){
	//int f = open("mpsh_aliases.txt", O_RDONLY );
	int f = open(alias_path1, O_RDONLY );

	if(f==-1)
		printf("impossible d'ouvrir le fichier\n");
	int i=0,j=0,k=0;
	char c=read(f,&c,1);
	char prec;
	char * s=malloc(SHELL_BUFFER*sizeof(char)); 
	char * res=malloc(SHELL_BUFFER*sizeof(char));
	while(c!=EOF){//Lors de ma boucle je ne trouve pas '\0', aucune idée de pourquoi
		while (c!='\n'){
	  		s[i]=c;
	  		i++;
	  		read(f,&c,1);
		}
		s[i]='\0';
		while(s[j]!=' ')
			j++;
		j++;//Saut de l'espace
		while(s[j]!='='){
			res[k]=s[j];
			k++;
			j++;
		}
		j++;//Saut de '='
		i=0;
		if(strcmp(comm,res)==0){
			while(s[j]!='\0'){
				fin[i]=s[j];
				j++;
				i++;
			}
			fin[i]='\0';
			return 1;
		}
		prec=c;
		read(f,&c,1);
		if(prec==c)
			return 0;
		i=0;
		j=0;
		k=0;
	}
	close(f);
	return 0;
}

//type
short type(char* command,char* alias_path1,char* alias_path2){
	char* cmdInterne[9] = {"cd", "exit", "pwd", "echo", "unmask", "history", "alias", "unalias","type"};
	for(int i=0;i<9;i++){
		if(strcmp(command,cmdInterne[i])==0){
			printf("%s est une commande primitive du shell\n",command);
			return 0;
		}
	}
	if(estAlias(command,alias_path1,alias_path2))
		return 1;
	return 2;
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
		if(e[i][j]=='=' && strncmp(s,tmp,j)==0){
			return i;
		}else
			return -2;
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
//redirection
int erreur(char * err)
{
    write(2, err, strlen(err));
    return 1;
}


void parse(char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char **ali,int nbali,char* alias_path1,char* alias_path2){
	char * res=malloc(SIZE*sizeof(char));
	int nbarg=nbargs(command);
	
	if(strcmp(command[0], "cd") == 0){
			if(strcmp(command[1],"~")==0){
				char * tmp="/home/";
				char * result= malloc(strlen(tmp)+strlen(getenv("USER"))+1);
				strcpy(result,tmp);
				strcat(result,getenv("USER"));
				cd(result);
			}else if(cd(command[1])<0){
				perror(command[1]);
			}			
		}else if(command[0][0]=='$'){
			char * tmp=malloc(SIZE*sizeof(char));
			char * tmp2=malloc(SIZE*sizeof(char));			
			tmp=parcoursexp((command[0])+1,exp,nbexp);
			if(strcmp(tmp,"Pas de export trouve !")==1){
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,ali,nbali,alias_path1,alias_path2);
			}else{
				tmp[0]='\0';
				tmp=parcoursexp((command[0])+1,ali,nbali);
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,ali,nbali,alias_path1,alias_path2);
			}
		}else if (egalecomm(command)==0){
			int i=ajoutExp(command[0],ali,nbali);
			if(i>=0)
					ali[i]=command[0];
				else{
					ali[nbali]=command[0];
					nbali++;
				}
		}else if (strcmp(command[0], "exit") == 0){
			exit(0);
		}else if(strcmp(command[0], "pwd") == 0){
			printf("%s\n", pwd());
		}else if(strcmp(command[0], "echo")==0){
			if(command[1][0]=='$'){
				char * tmp=malloc(SIZE*sizeof(char));
				tmp=concat(parcoursexp((command[1])+1,exp,nbexp),(resteDeLaCommande(command,2)));
				printf("%s\n", tmp);
			}else
				my_echo(nbarg,command);
		}else if(strcmp(command[0], "umask")==0){
			my_umask(nbarg,command);
		}else if(strcmp(command[0], "history")==0){
			history(nbarg,command,h,nbcom);
		}else if(strcmp(command[0],"alias")==0){
			my_alias(nbarg,command,alias_path1,alias_path2);
		}else if(strcmp(command[0],"unalias")==0){
			my_unalias(nbarg,command,alias_path1,alias_path2);
		}else if(strcmp(command[0],"type")==0){
			int t = type(command[1],alias_path1,alias_path2);
			if (t==2){
				printf("%s est /bin/%s\n",command[1],command[1]);
			}
		}else if(AliasComp(command[0],res,alias_path1,alias_path2)==1){
			char * tmp=res;
			for (int i=1;i<nbarg;i++){
				tmp=concat(tmp,command[i]);
			}
			free(command);
			command=read_input(tmp);
			parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,ali,nbali,alias_path1,alias_path2);
			
		}else if(strcmp(command[0],"export")==0){
			if(nbarg!=2){
				printf("Mauvais nombre d'argument\n");
			}else if (strcmp(command[1],"-n")==0){
				exportN(exp,nbexp);
			}else{
				int i = ajoutExp(command[1],exp,nbexp);
				if(i>=0)
					exp[i]=command[1];
				else{
					exp[nbexp]=command[1];
					nbexp++;
				}
			}
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

int redirection(char *dir,char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char **ali,int nbali,char* alias_path1,char* alias_path2)
{
	int t = type(command[0],alias_path1,alias_path2);
	int nbarg=nbargs(command);
	pid_t fils=fork();
    if(fils==0){
   		int file = open(dir, O_CREAT|O_WRONLY, 0666);
    	if(file == -1)
    	{
        // En cas d'erreur
      	  erreur("Erreur : open\n");
    	}
 	
    // On redirige 1 vers file
    	dup2(file,1);
 
    	close(file);
    // On execute parse (pour le momment que les commandes internes)
    	if (t==0){
    		parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,ali,nbali,alias_path1,alias_path2);
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

void proc(){
	int nbcom=0;
	char ** h= malloc(SIZE*sizeof(char*));
	int nbexp = 0;
	char ** exp = malloc(SIZE*sizeof(char*));
	int nbali = 0; //Du type alias=commande
	char ** ali = malloc(SIZE*sizeof(char*));
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
	char * alias_path1 = path_alias_txt("/mpsh_alias");
	char * alias_path2 = path_alias_txt("/newal");

	while(TRUE){
		int d=0;
		make_prompt();
		command = read_input(readline("~s "));
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
				redirection(dir,command,h,nbcom,child_pid,stat_loc,exp,nbexp,ali,nbali,alias_path1,alias_path2);
				d++;
			}
		}



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
				parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,ali,nbali,alias_path1,alias_path2);
				d++;
			}else{
				tmp=parcoursexp((command[0])+1,ali,nbali);
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,ali,nbali,alias_path1,alias_path2);
				d++;
			}
		}else if (egalecomm(command)==0 && d==0){
			int i=ajoutExp(command[0],ali,nbali);
			if(i>=0){
					ali[i]=command[0];
					d++;
			}
				else if (i==-1){
					ali[nbali]=command[0];
					nbali++;
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
		}else if(strcmp(command[0],"alias")==0 && d==0){
			my_alias(nbarg,command,alias_path1,alias_path2);
			d++;
		}else if(strcmp(command[0],"unalias")==0 && d==0){
			my_unalias(nbarg,command,alias_path1,alias_path2);
			d++;
		}else if(strcmp(command[0],"type")==0 && d==0){
			int t = type(command[1],alias_path1,alias_path2);
			if (t==2){
				printf("%s est /bin/%s\n",command[1],command[1]);
			}
			d++;
		}else if(AliasComp(command[0],res,alias_path1,alias_path2)==1 && d==0){
			d++;
			char * tmp=res;
			for (int i=1;i<nbarg;i++){
				tmp=concat(tmp,command[i]);
			}
			free(command);
			command=read_input(tmp);
			parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,ali,nbali,alias_path1,alias_path2);
			
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
				make_Dir(command[1]);
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
