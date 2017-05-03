#include "gerador.h"

int main(int argc, char *argv[]){
    
    //verifica argumentos entrada
    if (argc != 3){
        printf("Command usage: gerador <no. requests> <max time>\n");
        exit(1);
    }

    if(fifo_init()<0){
        printf("GERADOR: erro ao  inicializar fifos.\n");
        exit(1);
    }

    //abre canal entrada
    fd_fifo_entrada = open(FIFO_ENTRADA,O_WRONLY|O_TRUNC);
    if(fd_fifo_entrada < 0){
        printf("GERADOR: erro ao abrir fifo entrada.\n");
        exit(10);
    }

    //abre canal rejeitados
    fd_fifo_rejeitados = open(FIFO_REJEITADOS,O_RDONLY|O_TRUNC);
    if(fd_fifo_rejeitados < 0){
        printf("GERADOR: erro ao abrir fifo rejeitados.\n");
        exit(10);
    }


    PEDIDO tentativa1;
    tentativa1.serial_num = contador;
    tentativa1.sex = 'M';
    tentativa1.rejeicoes = 0;
    tentativa1.duration = 10;

    if(write(fd_fifo_entrada, &tentativa1, sizeof(PEDIDO)) < 0){
        printf("GERADOR: erro ao enviar tentativa1.\n");
    }



    if(fifo_destroy()<0){
        printf("GERADOR: erro ao apagar fifos.\n");
        exit(1);
    }
    return 0;
}