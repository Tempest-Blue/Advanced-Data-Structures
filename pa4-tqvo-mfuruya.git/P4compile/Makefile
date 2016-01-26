# A simple makefile for CSE 100 P4

CC=g++
PROF = -pg
CXXFLAGS= -g $(PROF) -O2  --std=c++11 -Wall -pedantic
LDFLAGS= -g $(PROF) -O2 --std=c++11 -pthread

all: bogtest

bogtest:  boggleplayer.o boggleutil.o

boggleplayer.o: boggleutil.h 	baseboggleplayer.h 	boggleplayer.h

boggleutil.o: boggleutil.h

clean:
	rm -f bogtest  *.o core*


