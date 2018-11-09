
CC = gcc

CFLAGS = -std=c11 -Wall -g

LDLIBS= -lm -lreadline

ALL = mpsh

all : $(ALL)

mpsh: mpsh.c

cleanall:
	rm -rf *~ $(ALL)
