# Makefile 
Executables =  sauna gerador
Binaries =  sauna.o gerador.o
CFLAGS = -Wall -pthread

all: $(Executables)

.c :
	gcc $(CFLAGS) $< -o $@ 

$(Binaries): Utils.h
sauna.o: sauna.h
gerador.o: gerador.h


clean:
	rm -f $(Executables) \
	$(Binaries)
#endOf Makefile
