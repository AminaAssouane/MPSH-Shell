
CC = gcc

CFLAGS = -std=c11 -Wall -g

LDLIBS= -lm -lreadline

ALL = mpsh

all : $(ALL)
mpsh: mpsh.o fonc_ls.o cat.o
mpsh.o: mpsh.c mpsh.h cat.h fonc_ls.h
cat.o: cat.c cat.h
fonc_ls.o: fonc_ls.c fonc_ls.h
cleanall:
	rm -rf *~ $(ALL)
