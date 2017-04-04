# Makefile example 2
#
# Two executables are to be created: bank , client
# Their source code is, respectively: bank.c , client.c
# The sources need, respectively: (common.h , bank.h) , common.h
# For testing, all needed files can be found in the current directory 
#
Executables =  main 
Binaries =  main.o 
CFLAGS = -Wall

all: $(Executables)

trial: main.o
	cc $(CFLAGS) main.o -o main

clean:
	rm -f $(Executables) \
		$(Binaries)
