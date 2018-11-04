
CC = gcc

CFLAGS =-Wall -g -lreadline

LDLIBS= -lm

ALL = mpsh

all : $(ALL)

mpsh: mpsh.c

cleanall:
	rm -rf *~ $(ALL)
