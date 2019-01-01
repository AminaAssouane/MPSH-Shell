int redirectionE(char *dir,char ** command,char ** h,int nbcom,pid_t child_pid,int stat_loc,char **exp,int nbexp,char** eg,int nbeg,char **ali,int nbali)
{
         int t = type(command[0]);
         int nbarg=nbargs(command);
  // La redirection d'entrée "<" marche comme les commandes elle même, elle ne fait qu'afficher ce que les commandes renvoie dans la sortie standard donc on n'a rien besoin d'ajouter de plus  et on execute les commandes directement
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
