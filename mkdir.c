#include "mkdir.h"
/**
 *  \function MakeDir
 *  \author PengWeizhe
 *  \date 
 *  \param [in] path the path need to creat
 *  \return 0 ok 1 failed
 *  \details make a multiple dir
 */
int makeDir(const char* path){
    int beginCmpPath;
    int endCmpPath;
    int fullPathLen;
    int pathLen = strlen(path);
    char currentPath[128] = {0};
    char fullPath[128] = {0};

    printf("path = %s\n", path);
    if('/' != path[0]){   
        //get the gurrent path
        getcwd(currentPath, sizeof(currentPath));
        strcat(currentPath, "/");
        printf("currentPath = %s\n", currentPath);
        beginCmpPath = strlen(currentPath);
        strcat(currentPath, path);
        if(path[pathLen] != '/'){
            strcat(currentPath, "/");
        }
        endCmpPath = strlen(currentPath);
        
    }
    else{
        int pathLen = strlen(path);
        strcpy(currentPath, path);
        if(path[pathLen] != '/')
        {
            strcat(currentPath, "/");
        }
        beginCmpPath = 1;
        endCmpPath = strlen(currentPath);
    }
    //make a child dir
    for(int i = beginCmpPath; i < endCmpPath ; i++ ){
        if('/' == currentPath[i])
        {
            currentPath[i] = '\0';
            if(access(currentPath, NULL) != 0)
            {
                if(mkdir(currentPath, 0755) == -1)
                {
                    printf("currentPath = %s\n", currentPath);
                    perror("mkdir error %s\n");
                    return -1;
                }
            }
            currentPath[i] = '/';
        }
    }
    return 0;
}
/*
int main(int argc, char* argv[]){
    //makeDir("helloworld");
    if(argc >=2 ){
    	makeDir(argv[1]);
    }
    return 0;
}
*/
