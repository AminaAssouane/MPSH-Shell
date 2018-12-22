#include "mkdir.h"

int make_Dir(const char* path){
    int bgPath;
    int endPath;
    int fullPathLen;
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
    return 1;
}

