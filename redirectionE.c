int redirectionE(char *dir,char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali)
{
         int t = type(command[0]);
         int nbarg=nbargs(command);
         pid_t fils=fork();
         if(fils==0){
   		    int file = open(dir, O_RDONLY, 0777);
    	    if(file == -1)
    	    { // En cas d'erreur
      	        erreur("Mpsh : try : no such file or directory \n");
    	    }
            
            // On redirige 1 vers file
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
		}else if (strcmp(command[0],"mkdir")==0){
			make_Dir(command[1]);
		}
    	close(1);
    	exit(1);
    }
    return 1;
}