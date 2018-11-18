#include "fonc_ls.h"


void print_error(const char *err_string, int line){
	fprintf(stderr, "line: %d", line);
	perror(err_string);
	exit(1);
}

void display_attribute(struct stat st, char * name){
	char buf_time[32];
	struct passwd *psd;	//get user name
	struct group *grp;	//get user groupe

	//print the type of fichier
	if(S_ISLNK(st.st_mode)){
		printf("l");
	}else if(S_ISREG(st.st_mode)){
		printf("-");
	}else if(S_ISDIR(st.st_mode)){
		printf("d");
	}else if(S_ISCHR(st.st_mode)){
		printf("c");
	}else if(S_ISBLK(st.st_mode)){
		printf("b");
	}else if(S_ISFIFO(st.st_mode)){
		printf("f");
	}else if(S_ISSOCK(st.st_mode)){
		printf("s");
	}

	//print the right of owner
	if(st.st_mode & S_IRUSR){	
		printf("r");
	}else{
		printf("-");
	}
	if(st.st_mode & S_IWUSR){
		printf("w");
	}else{
		printf("-");
	}

	if(st.st_mode & S_IXUSR){
		printf("x");
	}else{
		printf("-");
	}

	//print the right of the same groupe
	if(st.st_mode & S_IRGRP){
		printf("r");
	}else{
		printf("-");
	}

	if(st.st_mode & S_IWGRP){
		printf("w");
	}else{
		printf("-");
	}

	if(st.st_mode & S_IXGRP){
		printf("x");
	}else{
		printf("-");
	}

	//print the right of others
	
	if(st.st_mode & S_IROTH){
		printf("r");
	}else{
		printf("-");
	}

	if(st.st_mode & S_IWOTH){
		printf("w");
	}else{
		printf("-");
	}

	if(st.st_mode & S_IXOTH){
		printf("x");
	}else{
		printf("-");
	}

	printf("	");

	psd = getpwuid(st.st_uid);
	grp = getgrgid(st.st_gid);
	printf("%4d ", (int)st.st_nlink); //number of the links of a fichier
	printf("%-12s", psd->pw_name);
	printf("%-12s", grp->gr_name);

	printf("%6d", (int)st.st_size); //large of a fichier
	strcpy(buf_time, ctime(&st.st_mtime));
	buf_time[strlen(buf_time)-1] = '\0'; //supprimer the character of change line
	printf("    %s", buf_time);	//printf the time information
}

void display_single(char * name){
	int i;
	int lg;

	if(g_leave_len < g_maxlen){
		printf("\n");
		g_leave_len = MAXROWLEN;
	}

	lg = g_maxlen - strlen(name);
	printf("%s-s", name);

	for(i=0; i<lg;i++){
		printf(" ");
	}
	printf(" ");
	g_leave_len -= (g_maxlen+2); //leave two places
}

void display(int pam, char * pathname){

	int i;
	int j;
	struct stat buf;
	char name[NAME_MAX +1];

	//get the filename from the pathname
	for(i=0, j=0; i<strlen(pathname); i++){
		if(pathname[i]== '/'){
			j = 0;
			continue;
		}
		name[j++] = pathname[i];
	}
	name[j++] = '\0';

	if(lstat(pathname, &buf) == -1){
		printf("Not enough right \n");
		return ;
		print_error("stat", __LINE__);
	}

	switch(pam){
		case PARM_VIDE:			//sans argument
			if(name[0] != '.'){
				display_single(name);
			}
			break;
		case PARM_R:			// -r
			if(name[0] != '.'){
				display_single(name);
			}
			break;
		case PARM_A:
			display_single(name);
			break;
		case PARM_L:	//-l
			if(name[0] != '.'){
				display_attribute(buf,name);
				printf(" %-s\n", name);
			}
			break;
		case PARM_A + PARM_L: //-al
			display_attribute(buf, name);
			printf(" %-s\n",name);
			break;

		case PARM_R + PARM_A: //-ar
			display_single(name);
			break;
		case PARM_L + PARM_R: //-lr
			if(name[0] != '.'){
				display_attribute(buf, name);
				printf(" %-s\n", name);
			}
			break;

		default:
			break;	
	}
}

void display_dir(int pam, char* pathname){
	DIR *dp; //dir
	struct dirent *entre; //ptr
	int count = 0;
	char filename[600][PATH_MAX+1],temp[PATH_MAX + 1];

	if((dp = opendir(pathname)) == NULL){
		fprintf(stderr, "pas possible de ouverir ref: %s \n", pathname);
	}
	
	while((entre =readdir(dp)) != NULL){
        	if(g_maxlen < strlen(entre->d_name)){
            		g_maxlen = strlen(entre->d_name);
        	}	
        	count++;
    	}
    	closedir(dp);
	
	int i,j,lg = strlen(pathname);
	dp = opendir(pathname);
	for(int i=0; i<count; i++){
		entre = readdir(dp);
		if( entre == NULL){
			print_error("readdir", __LINE__);
		}
		strncpy(filename[i], pathname, lg);
		filename[i][lg] = '\0';
		strcat(filename[i], entre->d_name);
		filename[i][lg + strlen(entre->d_name)] = '\0';
	}
	closedir(dp);

	for(i=0; i<count; i++){
		display(pam, filename[i]);
	}

	if((pam & PARM_L) == 0){
		printf("\n");
	}
}

int ls_main_fonction(int argc, char **argv){
	printf("xixixi\n");
	printf("argc: %d \n", argc);
	//printf("command %s \n", argv[0]);
	
    	int i, j, k, num;
    	char param[32];     //保存命令行参数，目标文件和目录名不在此例
    	int flag_param = PARM_VIDE;    //参数种类，即是否有-l和-a选项
    	struct stat buf;

    	char *path = (char*)malloc(PATH_MAX+1);
    	//命令行参数的解析，分析-l,-a,-al,-la选项
    	j = 0;
    	num = 0;
    	for(i = 1; i < argc; i++){
        	if(argv[i][0] == '-'){
            		for(k = 1; k < strlen(argv[i]); k++, j++){
                		param[j] = argv[i][k];  //获取'-'后面的参数保存到数组param
            		}
            		num++;      //保存'-'的个数
		}
    	}


	void ls_R(int argc, char **argv){
    		int flag_param = 0;
    		struct stat buf;
    		int i;
    		char path[500];
    		for(i = 1; i < argc; i++){
        		if(argv[i][0] != '-') {
            			strcpy(path, argv[i]);
        		}
    		}
   	 	printf("\n%s:\n", path);
    		recursion(flag_param, path);
    		exit(0);
	}
    	//support r l a
    	for(i = 0; i < j; i++){
        	if(param[i] == 'a'){
            		flag_param |= PARM_A;
            		continue;
        	}else if(param[i] == 'l'){
            		flag_param |= PARM_L;
            		continue;
        	}else if(param[i] == 'r'){
            		flag_param |= PARM_R;
            		continue;
        	}else if(param[i] == 'R'){
            		free(path);
            		ls_R(argc, argv);
        	}else{
            		printf("my_ls: invalid opention -%c\n", param[i]);
            		exit(1);
		}
    	}
    	param[j] = '\0';

    //如果没有输入文件名或目录，就显示当前目录
    if((num + 1) == argc)
    {
        strcpy(path, "./");
        path[2] = '\0';
        display_dir(flag_param, path);
        return 0;
    }

    i = 1;
    do
    {
        //如果不是目标文件名或目录，解析下一个命令行参数
        if(argv[i][0] == '-')
        {
            i++;
            continue;
        }
        else 
        {
            strncpy(path, argv[i],strlen(argv[i])+1);

            if(stat(path, &buf) == -1)
            {
                print_error("stat", __LINE__);
            }
            if(S_ISDIR(buf.st_mode))    //argv[i]是一个目录
            {
                if(path[strlen(argv[i])-1] != '/')  //如果目录的最后一个字符不是'/',就加上'/'
                {
                    path[strlen(argv[i])] = '/';
                    path[strlen(argv[i]) + 1] = '\0';
                }
                else
                {
                    path[strlen(argv[i])] = '\0';
                }
                display_dir(flag_param, path);
                i++;
            }
            else    //argv[i]是一个文件
            {
                display(flag_param, path);
                i++;
            }
        }
    }while(i < argc);
    return 0;
}
int recursion(int flag, char *path)
{
    DIR *dir;
    struct dirent *ptr;     
    char pathname[10000];     //带有文件名称的路径
    int flag_param = 0;
    struct stat buf;
    int len, i;
    display_dir(flag_param, path);
    
    strcpy(pathname, path);
    dir = opendir(path);
    while(1)
    {
        ptr = readdir(dir);
        if(ptr == NULL)
        {
            closedir(dir);
            return 1;
        }
        if(!(strcmp(ptr->d_name,".") && (strcmp(ptr->d_name, "..") && ptr->d_name[0] != '.')))
        {
            continue;
        }
        strcat(pathname, ptr->d_name);
        lstat(pathname, &buf);
        if(S_ISDIR(buf.st_mode))
        {
            len = strlen(pathname);
            pathname[len] = '/';
            pathname[len + 1] = '\0';
            printf("\n%s:\n", pathname);
            g_maxlen = 0;
            g_leave_len = MAXROWLEN;
            recursion(flag, pathname);
        }
        for(i = strlen(path); i < len ; i++)
        {
            pathname[i] = '\0';
        }
    }


}

/*

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


void fonction_ls_eee(char * dir,int dep){
	DIR *dp;
	struct stat statbuf;
	struct dirent *entre;

	if((dp = opendir(dir)) == NULL){
		fprintf(stderr, "pas possible de ouverir ref: %s \n", dir);
	}

	chdir(dir);
	while((entre = readdir(dp)) != NULL){
		lstat(entre->d_name, &statbuf);
		printf("%*s%s\t", dep,"",entre->d_name);
	}
	
	chdir("..");
	closedir(dp);
	
}

int main(int argc, char* argv[]){

	char * topdir = ".";
	if(argc >=2){
		topdir = argv[1];
	}
	fonction_ls_eee(topdir, 1);
}

*/
