# A simple makefile for CSE 100 P2

#use g++ for everything
CC= g++  	

# include debugging symbols in object files,
# and enable all warnings
CXXFLAGS= -std=c++11 -O2 -g 
#-Wall

#include debugging symbols in executable
LDFLAGS= -g	

netplan: Main.cpp Graph.hpp Vertex.hpp Edge.hpp	
	$(CC) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:  
	rm -f netplan *.o core* *.exe
