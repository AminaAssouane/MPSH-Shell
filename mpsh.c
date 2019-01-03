/*
 *------------Université Paris Diderot--------------------
 * Filename: mpsh.h
 * Version: 1.0 Beta
 * Created: 16h32m 02/11/18
 * Author: Equipe 33 (Amina ASSOUANE Mickeal Darmon Yuchen BAI )
 *---------------------------------------------------------
 */

#include "mpsh.h"
int parse(char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali);


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

	cmd[index] = NULL;  //le dernier character est NUL
	return cmd;
}

/* la fonction is_pipe_ou_et_exits sert à vérifier si pipe | ou & exist 
 * lorsque pipe exits, il retourne 2, si & exists, il retourne 3
 * morsque | et & n'exist pas, retourne -1
 */
short is_pipe_ou_et_exist(char * input){

	int len = strlen(input);
	for(int i=0;i<len;i++){
		if(input[i] == '|'){
			return 2;
		}
		if(input[i] == '&'){
			return 3;
		}
	}

	return -1;
}

//sep pour choisir le charactere à séparer la commande
char** read_command_pipe_ou_et(char * input,char * sep){
	
	char ** cmds = malloc(SHELL_BUFFER*sizeof(char *));
	if(cmds == NULL){
		perror("read_commands_pipe : malloc failed ");
		exit(1);
	}

	char * tmp;
	int index = 0;
	tmp = strtok(input, sep);
	while(tmp != NULL){
		cmds[index] = tmp;
		index++;
		tmp = strtok(NULL,sep);
	}

	cmds[index] = NULL;
	return  cmds;
}

void make_prompt(char * pathCons){
	char pathname[SHELL_BUFFER];
	char username[SHELL_BUFFER];
	//char hostname[SHELL_BUFFER];

	struct passwd * pwd = getpwuid(getuid());
	strcpy(username, pwd->pw_name);
	//gethostname(hostname,SHELL_BUFFER);
	memset(pathname, 0, sizeof(pathname));
	getcwd(pathname, sizeof(pathname));		//obtenir l'adresse actuel

	char *res = malloc(sizeof(char)*512);
	char *dest = malloc(sizeof(char)*512);
	char * filename = pathCons;
	FILE *fp;
	if((fp=fopen(filename,"r"))==NULL){
		printf("Erreur : make_prompt openfile! \n");
	}
	int z=1;
	while(!feof(fp)){
		fgets(res,1024,fp);
		if(strncmp(res,"INVITE='",8)==0){
			z=0;			
			memmove(dest,res+8,strlen(res)-8);
			break;
		}
	}

	fclose(fp);

	int len = strlen(dest);
	dest[len-1] = '\0';
	dest[len-4] = '\0';
	
	char tmp[512] ="";
	for(int i=0;i<len-2;i++){
		if(dest[i]=='$'){ //des options de INVITE
			if(dest[i+1] == 'P'){
				strcat(tmp,pathname);
				i++;
			
			}else if(dest[i+1] == 'U'){
				strcat(tmp,username);
				i++;
			}else{
				tmp[strlen(tmp)] = dest[i];

			}
			
		}else{
			tmp[strlen(tmp)] = dest[i];
		}
	}	
	
	if(tmp[strlen(tmp)-1] == '\n'){
		tmp[strlen(tmp)-1] = '\0';
	}
	
	if(tmp[strlen(tmp)-2] != 0){
		tmp[strlen(tmp)-2] = 0;
	}
	
	dest = tmp;
	if(z==0){
		printf("%s",dest);
		fflush(stdout);				//renouvoler espace
	}else{
		printf("[%s-%s]~$",username,pathname);
		
		fflush(stdout);					//renouvoler espace
	}
}

int make_chemin(char * path){
	char chemin[SHELL_BUFFER];
	memset(chemin, 0, sizeof(chemin));
	getcwd(chemin, sizeof(chemin));
	
	FILE *fp;
	char * tmp = malloc(sizeof(char)*512);
	if((fp=fopen(path,"r"))==NULL){
		printf("Erreur : make_prompt openfile! \n");
	}
	short h=1;
	while(!feof(fp)){
		fgets(tmp,1024,fp);
		if(strncmp(tmp,"export CHEMIN=",14)==0){
			h=0;	
			break;
		}
	}
	free(tmp);
	fclose(fp);

	if(h!=0){
		fp=fopen(path,"a+");
		if(fp==NULL){
			printf("erreur make_chemin mpshrc\n");
		}
		char ch[128] = "export CHEMIN=";
		strcat(ch,chemin);
		fseek(fp,0,SEEK_END);
		fwrite(ch,strlen(ch),1,fp);
		fclose(fp);
	}
	return 1;
}

void accesible(){
	char chemin[SHELL_BUFFER];
	memset(chemin, 0, sizeof(chemin));
	getcwd(chemin, sizeof(chemin));

	char str[] = "PATH=$PATH:";
	strcat(str,chemin);

	if(execlp("export","export",str,NULL) == -1){
		//printf("failed\n");
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
int pwd(){
	char res[SIZE];
	printf("%s\n",getcwd(res,SIZE));
	return 1;
}

//umask
int my_umask (int argc, char **argv){
	if (argc == 2){
		if(strlen(argv[1])>3){
		printf("Droits impossible à changer (nombre incorrect)\n");
		return -1;
	}else{
		int n;
		n=atol(argv[1]);
		if (n>777)
			printf("Droits impossible à changer (nombre incorrect)\n");
		umask(n);
		return 1;
	}
	}else if (argc>2){
		printf("Trop d'arguments pour umask\n");
		return -1;
	}else 
		printf("Manque l'argument\n");
	return -1;
}

//echo
int my_echo (int argv,char ** argc){
    int i;
    for (i=1;i<argv;i++){
    	printf("%s ",argc[i]);
    }
    printf("\n");
    return 1;
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
			return -1;
		}
	}
	return -1;
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

int estAlias(char *comm,char ** ali,int nbali){
	int i=0;
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
			return 1;
		}
		i=0;
		tmp[i]='\0';
		k++;
	}
	return -1;
}

//type
short type(char* command,char ** ali,int nbali,char ** exp,int nbexp,char ** eg,int nbeg){
	char* cmdInterne[10] = {"cd", "exit", "pwd", "echo", "unmask", "history", "alias", "unalias","type","export"};
	char * cmdExterne[3] = {"ls","cat","mkdir"};
	for(int i=0;i<10;i++){
		if(strcmp(command,cmdInterne[i])==0){
			return 0;
		}
	}
	for (int i = 0 ; i<3;i++){
		if(strcmp(command,cmdExterne[i])==0){
			return 4;
		}
	}
	if(estAlias(command,ali,nbali)==1)
		return 1;
	else if (command[0]=='$' && estAlias(command+1,exp,nbexp)==1)
		return 2;
	else if (command[0]=='$' && estAlias(command+1,eg,nbeg)==1)
		return 3;
	return -1;
}


int aliasA(char ** e,int nbexp){
	int i;
	for (i=0;i<nbexp;i++){
			printf("alias ");
			printf("%s\n",e[i]);
	}
	return 1;
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
int exportN(char ** e,int nbexp){
	int i;
	for (i=0;i<nbexp;i++){
		printf("%s\n",e[i]);
	}
	return 1;
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
int dollar(char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali){
	int r;
	char * tmp=malloc(SIZE*sizeof(char));
	char * tmp2=malloc(SIZE*sizeof(char));			
	tmp=parcoursexp((command[0])+1,exp,nbexp);
	if(strcmp(tmp,"Pas de export trouve !")!=0){
		tmp2=resteDeLaCommande(command,1);
		tmp=concat(tmp,tmp2);
		free(command);
		command=read_input(tmp);
		r=parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
	}else{
		tmp=parcoursexp((command[0])+1,eg,nbeg);
		tmp2=resteDeLaCommande(command,1);
		tmp=concat(tmp,tmp2);
		free(command);
		command=read_input(tmp);
		r=parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
	}
	return r;
}
int parse(char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali){
	int r=-1;
	char * res=malloc(SIZE*sizeof(char));
	int nbarg=nbargs(command);
	int d=0;
	if(strcmp(command[0], "cd") == 0 && d==0){
			if((r=cd(command[1]))<0){
				perror(command[1]);
			}
			d++;
		}else if(command[0][0]=='$' && d==0){
			char * tmp=malloc(SIZE*sizeof(char));
			char * tmp2=malloc(SIZE*sizeof(char));			
			tmp=parcoursexp((command[0])+1,exp,nbexp);
			if(strcmp(tmp,"Pas de export trouve !")==1){
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				r=parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}else{
				tmp=parcoursexp((command[0])+1,eg,nbeg);
				tmp2=resteDeLaCommande(command,1);
				tmp=concat(tmp,tmp2);
				free(command);
				command=read_input(tmp);
				r=parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}
		}else if (egalecomm(command)==0 && d==0){
			int i=ajoutExp(command[0],eg,nbeg);
			if(i>=0){
					eg[i]=command[0];
					d++;
					r=1;
			}else if (i==-1){
				eg[nbeg]=command[0];
				nbeg++;
				d++;
				r=1;
			}else{
				printf("Ne peut pas être parser\n");
				d++;
				r=-1;
			}
		}else if (strcmp(command[0], "exit") == 0 && d==0){
			exit(0);
			r=1;
			d++;
		}else if(strcmp(command[0], "pwd") == 0 && d==0){
			r=pwd();
			d++;
		}else if(strcmp(command[0], "echo")==0 && d==0){
			if(command[1][0]=='$'){
				char * tmp=malloc(SIZE*sizeof(char));
				tmp=concat(parcoursexp((command[1])+1,exp,nbexp),(resteDeLaCommande(command,2)));
				printf("%s\n", tmp);
				d++;
				r=1;
			}else{
				r=my_echo(nbarg,command);
				d++;
			}
		}else if(strcmp(command[0], "umask")==0 && d==0){
			r=my_umask(nbarg,command);
			d++;
		}else if(strcmp(command[0], "history")==0 && d==0){
			r=history(nbarg,command,h,nbcom);
			d++;
		}else if(strcmp(command[0], "helpme")==0){
			printf("can't help you !\n");
		}else if(strcmp(command[0],"alias")==0 && d==0){
			d++;
			if(nbarg==1){
				r=aliasA(ali,nbali);
			}else if (nbarg==2){
				int i = ajoutExp(command[1],ali,nbali);
				printf("%d\n",i);
				if(i>=0){
					ali[i]=command[1];
					r=1;
				}
				else if (i==-1){
					ali[nbali]=command[1];
					nbali++;
					r=1;
				}					
			}else{
				printf("Trop d'arguments pour alias\n");
				r=-1;
			}
		}
		else if(strcmp(command[0],"unalias")==0 && d==0){
			r=unalias(ali,nbali,command[1]);
			d++;
		}else if(strcmp(command[0],"type")==0 && d==0){
			int t = type(command[1],ali,nbali,exp,nbexp,eg,nbeg);
			r=t;
			if (r==0){
				printf("%s est une primitive du shell\n",command[1]);
			}else if (r==1){
				char * res=malloc(SIZE*sizeof(char));
				AliasComp(command[1],res,ali,nbali);
				printf("%s est un alias de %s\n",command[1],res);
			}else if (r==2){
				char * res = malloc(SIZE*sizeof(char));
				AliasComp(command[1]+1,res,exp,nbexp);
				printf("%s est un alias de %s\n",res,res);
			}else if (r==3){
				char * res = malloc(SIZE*sizeof(char));
				AliasComp(command[1]+1,res,eg,nbeg);
				printf("%s est un alias de %s\n",res,res);
			}
			else if (r==4){
				printf("%s est /bin/%s\n",command[1],command[1]);
			}else
				printf("La command entré n'est pas gérée par mpsh\n");
			d++;
		}else if(strcmp(command[0],"export")==0 && d==0){
			d++;
			if(nbarg!=2){
				printf("Mauvais nombre d'argument\n");
			}else if (strcmp(command[1],"-n")==0){
				r=exportN(exp,nbexp);
			}else{
				int i = ajoutExp(command[1],exp,nbexp);
				if(i>=0){
					exp[i]=command[1];
					r=1;
				}
				else if (i==-1){
					exp[nbexp]=command[1];
					nbexp++;
					r=1;
				}else{
					printf("Ne peut pas être un export\n");					
					r=-1;
				}
			}
		}
		else if(AliasComp(command[0],res,ali,nbali)==1){
			char * tmp=res;
			for (int i=1;i<nbarg;i++){
				tmp=concat(tmp,command[i]);
			}
			free(command);
			command=read_input(tmp);
			r=parse(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
		}
		return r;
	}

void addCurrentPathToRc(){

	char currentPath[80];
	getcwd(currentPath,sizeof(currentPath));


	const char *filename = "currentPath/abc";
	int p = open(filename, O_APPEND);
	
	char *res = "export PATH=\"";
	strcat(res,currentPath);
	char *tail = ":\\$PATH";
	strcat(res,tail);

	write(p,res,strlen(res));
	close(p);
}

int redirectionS(char *dir,char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali)
{
	
	pid_t fils=fork();
	int t = type(command[0],ali,nbali,exp,nbexp,eg,nbeg);
	int nbarg=nbargs(command);
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
    	return 1;
    }
    return -1;
}

int redirectionSerr(char *dir,char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali)
{
	int t = type(command[0],ali,nbali,exp,nbexp,eg,nbeg);
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
    	return 1;
    }
    return -1;
}

int redirectionE(char *dir,char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali)
{
        int t = type(command[0],ali,nbali,exp,nbexp,eg,nbeg);
        int nbarg=nbargs(command);
        pid_t fils=fork();
        if(fils==0)
        {
   		    int file = open(dir, O_RDONLY, 0777);
    	    if(file == -1)
    	    { // En cas d'erreur
      	        erreur("Mpsh : try : no such file or directory \n");
      	        return -1;
      	        exit(-1);
    	    }
            
            // On redirige 0 vers file
            dup2(file,0); 
    	    close(file);

         // La redirection d'entrée "<" marche comme les commandes elle même, elle ne fait qu'afficher ce que les commandes renvoie dans la sortie standard
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
			}else if (strcmp(command[0],"mkdir")==0)
				make_Dir(command[1]);
		
    	close(0);
    	exit(1);

    	}else{
    		waitpid(fils,&stat_loc,WUNTRACED);
    	}
    return 1;
}

int proc_command_extern(char ** command, int nbarg, pid_t child_pid, int stat_loc, int r, int d){

	if(strcmp(command[0], "ls")==0 && d==0){
		d++;
		r=fonctionls_main(nbarg,command);
		return r;			
	}else if(strcmp(command[0],"cat")==0 && d==0){
		d++;
		if(nbarg>2){
			r=cat_n(nbarg,command);
			return r;
		}else{
			r=cat(command[1]);
			return r;
		}
	}else if (strcmp(command[0],"mkdir")==0 && d==0){
		d++;
		if(nbarg == 2){
			r=make_Dir(command[1]);
			return r;
		}else if(nbarg == 3 && strcmp(command[1],"-p")==0){
			printf("plusiers dirs");
			r=make_Dir(command[2]);
			//make_plu_Dirs(command[2]);
			return r;
		}
	}
	return -1;
}



int proc(){
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
	int r;
	char ** ali = malloc(SIZE*sizeof(char*));
	if(ali==NULL){
		printf("ali est dans le proc nulli avant while");
	}

	/*le boucle suitvant donne l'adresse de chemin actuel
	 */
	char path[SHELL_BUFFER];
	memset(path, 0, sizeof(path));
	getcwd(path, sizeof(path));
	strcat(path,"/mpshrc");

	make_chemin(path);
	accesible();

	while(TRUE){
		int d=0;
		make_prompt(path);

		char * cmd = readline(" ");		//cmd est input de string
		
		int vf=0;
		if(is_pipe_ou_et_exist(cmd) == 2){	//pour le pipe : '|'
			char** cmd_pipe = read_command_pipe_ou_et(cmd,"|");
			nbarg = nbargs(cmd_pipe);
			for(int i=0;i<nbarg;i++){
				char ** cmd_x = read_input(cmd_pipe[i]);
				int nbarg_x = nbargs(cmd_x);
				int tmp =0;
				int tmp2 = 0;
				if((tmp = parse(cmd_x,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali)) >=0 || 
				(tmp2 = proc_command_extern(cmd_x,nbarg_x,child_pid,stat_loc,r,d)) >=0){
				
					//printf("parse commande interne ; %d \n", tmp);
					//printf("parse commande externe ; %d \n", tmp2);
					vf=1;
					break;
				}

			}

			continue;
		}else if(is_pipe_ou_et_exist(cmd) == 3){	//pour le et : '&'
			char** cmd_et = read_command_pipe_ou_et(cmd,"&");	
			nbarg = nbargs(cmd_et);

			for(int i=0;i<nbarg;i++){
				//printf("xxx &&&&\n");
				char ** cmd_x = read_input(cmd_et[i]);
				int nbarg_x = nbargs(cmd_x);
				parse(cmd_x,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				proc_command_extern(cmd_x,nbarg_x,child_pid,stat_loc,r,d);
			}

			continue;
		}
		
		if(vf>0){
			continue;
		}

		command = read_input(cmd);
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
				char ** cmd_tmp;
				cmd_tmp=read_input(tmp);
				redirectionS(dir,cmd_tmp,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}
		}

		for (int cmp=1;cmp<nbarg;cmp++){
			if(command[cmp][0]=='<'){
				char * tmp = malloc(SIZE*sizeof(char));
				char * dir = command[cmp+1];
				for(int cmp2=0;cmp2<nbarg;cmp2++){
					if(strcmp(command[cmp2],"<")!=0){
						if(cmp2==0)
							strcpy(tmp,command[cmp2]);
						else{
							tmp=concat(tmp,command[cmp2]);
						}
					}

				}
				char ** cmd_tmp;
				cmd_tmp=read_input(tmp);
				redirectionE(dir,cmd_tmp,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}
		}
		for(int i = 0;i<nbarg;i++){
			if(strcmp(command[i],"2>")==0){
				char * tmp = malloc(SIZE*sizeof(char));
				char * dir = command[i+1];
				for(int cmp2=0;cmp2<nbarg;cmp2++){
					if(strcmp(command[cmp2],"2>")!=0){
						if(cmp2==0)
							strcpy(tmp,command[cmp2]);
						else
							tmp=concat(tmp,command[cmp2]);
					}else 
						break;
				}
				char ** cmd_tmp;
				cmd_tmp=read_input(tmp);
				redirectionSerr(dir,cmd_tmp,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
				d++;
			}
		}

		if(strcmp(command[0], "cd") == 0 && d==0){
			if(cd(command[1])<0){
				perror(command[1]);
			}
			d++;
		}else if(strcmp(command[0],"?")==0){
			printf("%d\n",r);
			d++;
		}else if(command[0][0]=='$' && d==0){
			r=dollar(command,h,nbcom,child_pid,stat_loc,exp,nbexp,eg,nbeg,ali,nbali);
			d++;
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
			r=pwd();
			d++;
		}else if(strcmp(command[0], "echo")==0 && d==0){
			if(command[1][0]=='$'){
				char * tmp=malloc(SIZE*sizeof(char));
				tmp=concat(parcoursexp((command[1])+1,exp,nbexp),(resteDeLaCommande(command,2)));
				printf("%s\n", tmp);
				d++;
			}else{
				r=my_echo(nbarg,command);
				d++;
			}
		}else if(strcmp(command[0], "umask")==0 && d==0){
			r=my_umask(nbarg,command);
			d++;
		}else if(strcmp(command[0], "history")==0 && d==0){
			r=history(nbarg,command,h,nbcom);
			d++;
		}else if(strcmp(command[0],"alias")==0 && d==0){
			d++;
			if(nbarg==1){
				r=aliasA(ali,nbali);
			}else if (nbarg==2){
				int i = ajoutExp(command[1],ali,nbali);
				if(i>=0){
					ali[i]=command[1];
					r=1;
				}else if (i==-1){
					ali[nbali]=command[1];
					nbali++;
					r=1;
				}					
			}else{
				printf("Trop d'arguments pour alias\n");
				r=-1;
			}
			//alias(command,ali,nbali);
		}
		else if(strcmp(command[0],"unalias")==0 && d==0){
			r=unalias(ali,nbali,command[1]);
			nbali--;
			d++;
		}else if(strcmp(command[0],"type")==0 && d==0){
			int r = type(command[1],ali,nbali,exp,nbexp,eg,nbeg);
			if (r==0){
				printf("%s est une primitive du shell\n",command[1]);
			}else if (r==1){
				char * res=malloc(SIZE*sizeof(char));
				AliasComp(command[1],res,ali,nbali);
				printf("%s est un alias de %s\n",command[1],res);
			}else if (r==2){
				char * res = malloc(SIZE*sizeof(char));
				AliasComp(command[1]+1,res,exp,nbexp);
				printf("%s est un alias de %s\n",res,res);
			}else if (r==3){
				char * res = malloc(SIZE*sizeof(char));
				AliasComp(command[1]+1,res,eg,nbeg);
				printf("%s est un alias de %s\n",res,res);
			}
			else if (r==4){
				printf("%s est /bin/%s\n",command[1],command[1]);
			}else
				printf("La command entré n'est pas gérée par mpsh\n");
			d++;
		}else if(strcmp(command[0],"export")==0 && d==0){
			d++;
			if(nbarg > 2){
				printf("Mauvais nombre d'argument\n");
			}else if(nbarg == 1){
				//fair quelques chose
				printf("liere le fin du fichier mpshrc\n");
			}else if (strcmp(command[1],"-n")==0){
				r=exportN(exp,nbexp);
			}else if (strncmp(command[1], "CHEMIN=", 7) == 0){
				printf("ainyou \n");
			}else{
				int i = ajoutExp(command[1],exp,nbexp);
				if(i>=0){
					exp[i]=command[1];
					r=1;
				}else if (i==-1){
					exp[nbexp]=command[1];
					nbexp++;
					r=1;
				}else{
					printf("Ne peut pas être un export\n");					
					r=-1;
				}
			}
		}
		else if((r=AliasComp(command[0],res,ali,nbali))==1){
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
				r=fonctionls_main(nbarg,command);				
			}else if(strcmp(command[0],"cat")==0 && d==0){
				d++;
				if(nbarg>2){
					r=cat_n(nbarg,command);
				}else{
					r=cat(command[1]);
				}
			}else if (strcmp(command[0],"mkdir")==0 && d==0){
				d++;
				if(nbarg == 2){
					r=make_Dir(command[1]);
				}else if(nbarg == 3 && strcmp(command[1],"-p")==0){
					printf("plusiers dirs");
					r=make_Dir(command[2]);
				}
			}
			exit(1);
		}else{
			waitpid(child_pid, &stat_loc, WUNTRACED);
		}
		if(d==0)
			//erreur("Erreur: commande inconnue\n");
		d=0;
		free(command);
	}
}

int main(int argc, char * argv[]){
	proc();
	exit(0);
}
