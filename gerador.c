#include "gerador.h"

int main(int argc, char *argv[]){
    
    if (argc != 4){
        printf("Command usage: gerador <no. requests> <max time> <time unit>\n");
        exit(1);
    }

    char *unidade = argv[3];
    if (!(unidade[0] == 'u' || unidade[0] == 's' || unidade[0] == 'm')){
        printf("Time unit usage: 's' for seconds, 'm' for milliseconds, 'u' for microseconds\n");
        exit(2);
    }
}