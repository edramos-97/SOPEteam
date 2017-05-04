#include "sauna.h"


void * thread_principal(void * argumento){ //le da entrada
    return NULL;
}


void * thread_espera(void * argumento){ //argumento sera o tempo a esperar
    return NULL;
}

//argv[1]-numero de pedidos a enviar
int main(int argc, char *argv[]){
    //relogio inicial
    clock_gettime(CLOCK_MONOTONIC_RAW,&time_init);

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

    //le numero vagas da sauna e atualiza os livres
    numero_vagas = (unsigned int)atoi(argv[1]);
    lugares_livres = numero_vagas;

    //prepara ficheiro controlo
    sprintf(nome_ficheiro_controlo,"%s%d",SUFIXO_CONTROLO_S,getpid());
    fd_controlo_s = open(nome_ficheiro_controlo,O_WRONLY|O_TRUNC|O_CREAT,PERMISSOES_MODE);




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


    //fechar ficheiro de controlo + estatisticas
    if(close(fd_controlo_s) < 0){
        perror("SAUNA: erro ao fechar ficheiro de controlo");
    }
    //destruir fifos
   if(fifo_destroy()<0){
        printf("SAUNA: erro ao apagar fifos.\n");
        exit(1);
    }
    return 0;
}


