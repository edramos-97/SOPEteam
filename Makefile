# Makefile example 2
#
# Two executables are to be created: bank , client
# Their source code is, respectively: bank.c , client.c
# The sources need, respectively: (common.h , bank.h) , common.h
# For testing, all needed files can be found in the current directory 
#
Executables =  trial 
Binaries =  trial.o 
CFLAGS = -Wall

all: $(Executables)

trial: trial.o
	cc $(CFLAGS) trial.o -o trial

clean:
	rm -f $(Executables) \
		$(Binaries)
