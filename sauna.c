#include "sauna.h"

int main(int argc, char *argv[]){
    
    //verifica argumentos entrada
    if (argc != 2){
        printf("Command usage: gerador <no. seats>\n");
        exit(1);
    }

    //criar fifos
    if(fifo_init()<0){
        printf("SAUNA: erro ao  inicializar fifos.\n");
        exit(1);
    }

    //abre canal de entrada
    fd_fifo_entrada = open(FIFO_ENTRADA,O_RDONLY|O_TRUNC);
    if(fd_fifo_entrada < 0){
        printf("SAUNA: erro ao abrir fifo entrada.\n");
        exit(10);
    }
    //abre cana rejeitados
    fd_fifo_rejeitados = open(FIFO_REJEITADOS,O_WRONLY|O_TRUNC);
    if(fd_fifo_rejeitados < 0){
        printf("SAUNA: erro ao abrir fifo rejeitados.\n");
        exit(10);
    }

    // PEDIDO tentativa1;
    // while(1){
    //     int size_read = sizeof(PEDIDO);
    //     size_read = read(fd_fifo_entrada, &tentativa1, sizeof(PEDIDO));
    //     if(size_read != sizeof(PEDIDO))
    //         continue;
    //     else {
    //         printf("SAUNA: recebi 1 pedido.\n");
    //         break;
    //     }
    // }



    //fechar as pontas dos fifos

    //destruir fifos
   if(fifo_destroy()<0){
        printf("SAUNA: erro ao apagar fifos.\n");
        exit(1);
    }
    return 0;
}


