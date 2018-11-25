
CC = gcc

CFLAGS = -std=c11 -Wall -g

LDLIBS= -lm -lreadline

ALL = mpsh

all : $(ALL)
mpsh: mpsh.o fonctionls.o cat.o mkdir.o
mpsh.o: mpsh.c mpsh.h cat.h fonctionls.h
fonctionls.o: fonctionls.c fonctionls.h
cat.o: cat.c cat.h
mkdir.o : mkdir.c mkdir.h
cleanall:
	rm -rf *~ $(ALL)
