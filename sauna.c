#include "sauna.h"

int main(int argc, char *argv[]){
    
    if (argc != 3){
        printf("Command usage: gerador <no. seats> <time unit>\n");
        exit(1);
    }

    char* unidade = argv[2];
    if (!(unidade[0] == 'u' || unidade[0] == 's' || unidade[0] == 'm')){
        printf("Time unit usage: 's' for seconds, 'm' for milliseconds, 'u' for microseconds\n");
        exit(2);
    }

}


