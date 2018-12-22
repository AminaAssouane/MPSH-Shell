#include "fonctionls.h"

// show the details of every file
void fichier_info(char* filename, struct stat* info_p){
    char* uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void tran_mode_charac();
    char modestr[11];

    tran_mode_charac(info_p->st_mode, modestr);

    printf("%s", modestr);
    printf(" %4d", (int) info_p->st_nlink);
    printf(" %-8s", uid_to_name(info_p->st_uid));
    printf(" %-8s", gid_to_name(info_p->st_gid));
    printf(" %8ld", (long) info_p->st_size);
    printf(" %.12s", 4 + ctime(&info_p->st_mtime));
    printf(" %s\n", filename);
}

void tran_mode_charac(int mode, char str[]){
    strcpy(str, "----------");

    if (S_ISDIR(mode)){
        str[0] = 'd';
    }

    if (S_ISCHR(mode)){
        str[0] = 'c';
    }

    if (S_ISBLK(mode)){
        str[0] = 'b';
    }

    if ((mode & S_IRUSR)){
        str[1] = 'r';
    }

    if ((mode & S_IWUSR)){
        str[2] = 'w';
    }

    if ((mode & S_IXUSR)){
        str[3] = 'x';
    }

    if ((mode & S_IRGRP)){
        str[4] = 'r';
    }

    if ((mode & S_IWGRP)){
        str[5] = 'w';
    }

    if ((mode & S_IXGRP)){
        str[6] = 'x';
    }

    if ((mode & S_IROTH)){
        str[7] = 'r';
    }

    if ((mode & S_IWOTH)){
        str[8] = 'w';
    }

    if ((mode & S_IXOTH)){
        str[9] = 'x';
    }
}

char* uid_to_name(uid_t uid){
    struct passwd* getpwuid(),* pw_ptr;
    static char numstr[10];

    if((pw_ptr = getpwuid(uid)) == NULL){
        sprintf(numstr,"%d",uid);

        return numstr;
    }
    else{
        return pw_ptr->pw_name;
    }
}

char* gid_to_name(gid_t gid){
    struct group* getgrgid(),* grp_ptr;
    static char numstr[10];

    if(( grp_ptr = getgrgid(gid)) == NULL){
        sprintf(numstr,"%d",gid);
        return numstr;
    }
    else{
        return grp_ptr->gr_name;
    }
}

void do_ls(char dirname[],int mode){
    DIR* dir_ptr;
    struct dirent* direntp;

    if ((dir_ptr = opendir(dirname)) == NULL){
        fprintf(stderr, "ls2: cannot open %s \n", dirname);
    }
    else{
        if(mode==OP_D){
            printf("%s   ", dirname);
        }
        else{
            char dirs[20][100];
            int dir_count = 0;
            
            while ((direntp = readdir(dir_ptr)) != NULL){

                if(mode < 200 && direntp->d_name[0]=='.'){
                    continue;
                }

                char complete_d_name[200];  //
                strcpy (complete_d_name,dirname);
                strcat (complete_d_name,"/");
                strcat (complete_d_name,direntp->d_name);
                
                struct stat info;
                if (stat(complete_d_name, &info) == -1){
                    perror(complete_d_name);
                }
                else{
                    if(mode == OP_L||mode == OP_AL){
                        fichier_info(direntp->d_name, &info);
                    }
                    else if(mode == OP_A||mode == OP_NONE||mode == OP_I||mode == OP_AI){
                        
			    if(mode == OP_I||mode == OP_AI){
                            printf("%llu ", direntp->d_ino);
                        }

                        printf("%s\t", direntp->d_name);
                    }
                    else if(mode == OP_R){

                        if(S_ISDIR(info.st_mode)){
                            printf("%s   ", direntp->d_name);

                            strcpy (dirs[dir_count],complete_d_name);
                            dir_count++;
                        }
                        else{
                            printf("%s   ", direntp->d_name);
                        }
                    }
                }
            }
	    printf("\n");

            if(mode == OP_R){
                int i=0;
                printf("\n");
                for(;i<dir_count;i++){
                    printf("%s:\n", dirs[i]);
                    do_ls(dirs[i],OP_R);
                    printf("\n");
                }
            }

        }

        closedir(dir_ptr);
    }
}

// analyse des options (-l，-i)
int analyzeParam(char* input){
    if(strlen(input)==2)
    {
        if(input[1]=='l') return OP_L;
        if(input[1]=='a') return OP_A;
        if(input[1]=='d') return OP_D;
        if(input[1]=='R') return OP_R;
        if(input[1]=='i') return OP_I;
    }
    else if(strlen(input)==3)
    {
        if(input[1]=='a'&& input[2]=='l') return OP_AL;
        if(input[1]=='a'&& input[2]=='i') return OP_AI;
    }
    return -1;
}

int fonctionls_main(int ac,char* av[]){

    if(ac == 1){
        do_ls(".",OP_NONE);
    }
    else{
        int mode = OP_NONE; // ls sans option 
        int have_file_param = 0;

        while(ac>1){
            ac--;
            av++;

            int calMode = analyzeParam(*av);
            if(calMode!=-1){
                mode+=calMode;
            }
            else{
                have_file_param = 1;
                do{
                    printf("%s:\n", *av);
                    do_ls(*av,mode);
                    printf("\n");

                    ac--;
                    av++;
                }while(ac>=1);
            }
        }//fin while

        if (!have_file_param){
            do_ls(".",mode);
        }
        
    }   
  	return 1;  
}

