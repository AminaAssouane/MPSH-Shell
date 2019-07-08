#include "mkdir.h"

int make_Dir(char* path){

    int bgPath;
    int endPath;
    //int fullPathLen;
    int pathLen = strlen(path);
    char currentPath[128] = {0};

    printf("path = %s\n", path);
    if('/' != path[0]){   
	    getcwd(currentPath, sizeof(currentPath));
	    strcat(currentPath, "/");
	    printf("currentPath = %s\n", currentPath);
	    bgPath = strlen(currentPath);
	    strcat(currentPath, path);
	    if(path[pathLen] != '/'){
            	strcat(currentPath, "/");
	    }
	    endPath = strlen(currentPath);
    }
    else{
        strcpy(currentPath, path);
        if(path[pathLen] != '/'){
            strcat(currentPath, "/");
        }
        bgPath = 1;
        endPath = strlen(currentPath);
    }
    
    for(int i = bgPath; i < endPath ; i++ ){
        if('/' == currentPath[i])
        {
            currentPath[i] = '\0';
            if(access(currentPath, NULL) != 0)
            {
                if(mkdir(currentPath, 0777) == -1)
                {
                    printf("currentPath = %s\n", currentPath);
                    perror("mkdir error %s\n");
                    return -1;
                }
            }
            currentPath[i] = '/';
        }
    }
    return 1;
}

int createMultiLevelDir(char* sPathName)  
    {  
        char DirName[256];      
        int i, len;
        
        strcpy(DirName, sPathName);      
        len = strlen(DirName);             
        if('/' != DirName[len-1]) {  
            strcat(DirName, "/"); 
            len++;
        }           
              
        for(i=1; i<len; i++)      
        {      
            if('/' == DirName[i])      
            {      
                DirName[i] = '\0';      
                if(access(DirName, F_OK) != 0)      
                {      
                    if(mkdir(DirName, 0777) == -1)      
                    {       
                        perror("mkdir() failed!");       
                        return -1;       
                    }      
                }    
                DirName[i] = '/';      
             }      
      }    
          
      return 0;      
    }  
    
int make_plu_Dirs(char * path){
           if(-1 == createMultiLevelDir(path)) {
               perror("CreateMultiLevelDir() failed!");
               return 0;
           }
        printf("directroy \"%s\" created sucessfully!\n", path);
        
        return 1;
}
int mkdirs(char *muldir) 
{
    int i,len;
    char str[512];    
    strncpy(str, muldir, 512);
    len=strlen(str);
    for( i=0; i<len; i++ )
    {
        if( str[i]=='/' )
        {
            str[i] = '\0';
            if( access(str,0)!=0 )
            {
                mkdir( str, 0777 );
            }
            str[i]='/';
        }
    }
    if( len>0 && access(str,0)!=0 )
    {
        mkdir( str, 0777 );
    }
    return 1;
}

