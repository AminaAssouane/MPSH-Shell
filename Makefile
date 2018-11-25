
CC = gcc

CFLAGS = -std=c11 -Wall -g

LDLIBS= -lm -lreadline

ALL = mpsh

all : $(ALL)
mpsh: mpsh.o fonctionls.o cat.o
mpsh.o: mpsh.c mpsh.h cat.h fonctionls.h
cat.o: cat.c cat.h
fonctionls.o: fonctionls.c fonctionls.h
cleanall:
	rm -rf *~ $(ALL)
