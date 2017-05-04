#include "gerador.h"


void * thread_trata_rejeitados(void * argument){



    return NULL;
}

void envia_pedido(PEDIDO p){
    if(write(fd_fifo_entrada, &p, sizeof(PEDIDO)) <= 0){
        printf("GERADOR: erro ao enviar o pedido com numero de serie %d\n", p.serial_num);
    }
    else{
        //Campos da mensagem têm tamanhos constantes.
        //tempo de referencia é o inicio do programa.
        //(instante de tempo em ms) – (pid do processo) – (numero do pedido): (genero do utilizador) – (duração de utilização) – (tipo de msg)
        //tipos de mensagem "PEDIDO", "REJEITADO" ou "DESCARTADO"
        fprintf(FILE *stream, "%8sms - ", convertToMilisecond());//tempo
        fprintf(FILE *stream, "%-5s - ", getpid());//pid proc
        fprintf(FILE *stream, "%-4s: ", p.serial_num);//num pedidos
        fprintf(FILE *stream, "%c - ", p.sex);//genero
        fprintf(FILE *stream, "%-6s - ", p.duration);//duração
        fprintf(FILE *stream, "%-10s\n", "PEDIDO");//tipo
    }

    return;
}


void * thread_geradora(void * argument){
    int i,randno;
    for(i = 0; i < numero_gerador; i++){
        //gera pedido random
        randno = rand();
        PEDIDO pedido_curr;
        pedido_curr.serial_num = contador;
        pedido_curr.sex = (randno % 2) ? 'M' : 'F';
        pedido_curr.rejeicoes = 0;
        pedido_curr.duration = (randno % numero_max_tempo)+1;
        //envia pedido random
        envia_pedido(pedido_curr);
        contador++;
    }
    return NULL;
}



//argv[1]-numero de pedidos a enviar
//argv[2]-tempo max. de utilização (ms)
int main(int argc, char *argv[]){
    //seed para os rands
    srand(time(NULL));
    //relogio inicial
    clock_gettime(CLOCK_MONOTONIC_RAW,&time_init);

    //verifica argumentos entrada
    if (argc != 3){
        printf("Command usage: gerador <no. requests> <max time>\n");
        exit(1);
    }
    //criar fifos
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

    //pedidos a gerar
    numero_gerador = (unsigned int)atoi(argv[1]);
    //maximo de tempo para cada pedido, milisegundos
    numero_max_tempo = (unsigned int)atoi(argv[2]);

    //prepara ficheiro controlo
    sprintf(nome_ficheiro_controlo,"%s%d",SUFIXO_CONTROLO_G,getpid());
    fd_controlo_g = open(nome_ficheiro_controlo,O_WRONLY|O_TRUNC|O_CREAT,PERMISSOES_MODE);

    //função para thread de geração
    //gera pedidos aleatórios e envia para a sauna
    //regista no ficheiro ger.(pid do processo) os pedidos enviados


    //função para thread de tratar pedidos rejeitados
    //verifica o numeros de vezes que o pedido foi rejeitado e recoloca-o na fila
    //de pedidos ou descarta-o caso já tenha sido rejeitado 3 vezes
    //regista no ficheiro os pedidos rejeitados e os pedidos discartados


    //Campos da mensagem têm tamanhos constantes.
    //tempo de referencia é o inicio do programa.
    //(instante de tempo em ms) – (pid do processo) – (numero do pedido): (genero do utilizador) – (duração de utilização) – (tipo de msg)
    //tipos de mensagem "PEDIDO", "REJEITADO" ou "DESCARTADO"





    


    //fechar as pontas dos fifos


    //fechar ficheiro de controlo + estatisticas
    if(close(fd_controlo_g) < 0){
        perror("GERADOR: erro ao fechar ficheiro de controlo");
    }
    //destruir fifos
    if(fifo_destroy()<0){
        printf("GERADOR: erro ao apagar fifos.\n");
        exit(1);
    }
    return 0;
}