#include "mpsh.h"

int nbcom=0;
int nbali = 0;
int nbexp = 0;
char ** h;
char ** ali;
char ** expo;

void init(){
	h=malloc(SIZE*sizeof(char*));
	ali=malloc(SIZE*sizeof(char*));
	expo=malloc(SIZE*sizeof(char*));

}

int main(){
	init();
	proc();
	return 1;
}