# Makefile 
Executables = sauna gerador
Binaries =  sauna.o gerador.o 
CFLAGS = -Wall
LFLAGS = -lpthread

all: $(Executables)

sauna:	sauna.c Utils.h
	gcc $(CFLAGS) sauna.c Utils.h -o sauna $(LFLAGS)
gerador: gerador.c Utils.h
	gcc $(CFLAGS) gerador.c Utils.h -o gerador $(LFLAGS)
clean:
	rm -f $(Executables) \
	$(Binaries)
#endOf Makefile
