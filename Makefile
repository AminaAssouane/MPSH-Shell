
CC = gcc

CFLAGS = -std=c11 -Wall -g

LDLIBS= -lm -lreadline

ALL = mpsh

all : $(ALL)

mpsh: fonc_ls.h mpsh.h

fonc_ls.o: fonc_ls.c fonc_ls.h

cleanall:
	rm -rf *~ $(ALL)
