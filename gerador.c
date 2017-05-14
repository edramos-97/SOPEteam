#include "gerador.h"

void *thread_trata_rejeitados(void *argument)
{
    PEDIDO pedido_rejeitado;
    int resultado_leitura = read(fd_fifo_rejeitados, &pedido_rejeitado, sizeof(pedido_rejeitado));
    while (resultado_leitura > 0)
    {
        if (pedido_rejeitado.sex == 'F')
            estat_rejeitados_f++;
        else
            estat_rejeitados_m++;

        sem_wait(&semaforo_controlo);
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_curr_g);
        dprintf(fd_controlo_g, "%10.2fms - ", convertToMilliseconds(time_curr_g) - convertToMilliseconds(time_init_g)); //tempo
        dprintf(fd_controlo_g, "%-5d - ", getpid());                                                                //pid proc
        dprintf(fd_controlo_g, "%-10u: ", pedido_rejeitado.serial_num);                                                            //num pedidos
        dprintf(fd_controlo_g, "%c - ", pedido_rejeitado.sex);                                                                     //genero
        dprintf(fd_controlo_g, "%-6d - ", pedido_rejeitado.duration);                                                              //duração
        dprintf(fd_controlo_g, "%-10s\n", "REJEITADO");
        sem_post(&semaforo_controlo);

        pedido_rejeitado.rejeicoes++;
        if (pedido_rejeitado.rejeicoes < 3)
        { //envia para entrada
            sem_wait(&semaforo_controlo);
            clock_gettime(CLOCK_MONOTONIC_RAW, &time_curr_g);
            dprintf(fd_controlo_g, "%10.2fms - ", convertToMilliseconds(time_curr_g) - convertToMilliseconds(time_init_g)); //tempo
            dprintf(fd_controlo_g, "%-5d - ", getpid());                                                                //pid proc
            dprintf(fd_controlo_g, "%-10u: ", pedido_rejeitado.serial_num);                                                            //num pedidos
            dprintf(fd_controlo_g, "%c - ", pedido_rejeitado.sex);                                                                     //genero
            dprintf(fd_controlo_g, "%-6d - ", pedido_rejeitado.duration);                                                              //duração
            dprintf(fd_controlo_g, "%-10s\n", "PEDIDO");
            write(fd_fifo_entrada, &pedido_rejeitado, sizeof(pedido_rejeitado));
            sem_post(&semaforo_controlo);
        }
        else
        { //descarta
            if (pedido_rejeitado.sex == 'F')
                estat_descartados_f++;
            else
                estat_descartados_m++;
            sem_wait(&semaforo_controlo);
            clock_gettime(CLOCK_MONOTONIC_RAW, &time_curr_g);
            dprintf(fd_controlo_g, "%10.2fms - ", convertToMilliseconds(time_curr_g) - convertToMilliseconds(time_init_g)); //tempo
            dprintf(fd_controlo_g, "%-5d - ", getpid());                                                                //pid proc
            dprintf(fd_controlo_g, "%-10u: ", pedido_rejeitado.serial_num);                                                            //num pedidos
            dprintf(fd_controlo_g, "%c - ", pedido_rejeitado.sex);                                                                     //genero
            dprintf(fd_controlo_g, "%-6d - ", pedido_rejeitado.duration);                                                              //duração
            dprintf(fd_controlo_g, "%-10s\n", "DESCARTADO");
            sem_post(&semaforo_controlo);
        }

        resultado_leitura = read(fd_fifo_rejeitados, &pedido_rejeitado, sizeof(pedido_rejeitado));
    }

    //fifo rejeitados foi fechado entao fecha fifo entrada;
    close(fd_fifo_entrada);
    close(fd_fifo_rejeitados);

    return NULL;
}

void envia_pedido(PEDIDO p)
{
    if (write(fd_fifo_entrada, &p, sizeof(PEDIDO)) <= 0)
    {
        printf("GERADOR: erro ao enviar o pedido com numero de serie %d\n", p.serial_num);
    }
    else
    {
        //atualiza estatisticas
        if (p.sex == 'F')
            estat_gerados_f++;
        else
            estat_gerados_m++;

        //Campos da mensagem têm tamanhos constantes.
        //tempo de referencia é o inicio do programa.
        //(instante de tempo em ms) – (pid do processo) – (numero do pedido): (genero do utilizador) – (duração de utilização) – (tipo de msg)
        //tipos de mensagem "PEDIDO", "REJEITADO" ou "DESCARTADO"
        sem_wait(&semaforo_controlo);
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_curr_g);
        dprintf(fd_controlo_g, "%10.2fms - ", convertToMilliseconds(time_curr_g) - convertToMilliseconds(time_init_g)); //tempo
        dprintf(fd_controlo_g, "%-5d - ", getpid());                                                                //pid proc
        dprintf(fd_controlo_g, "%-10u: ", p.serial_num);                                                            //num pedidos
        dprintf(fd_controlo_g, "%c - ", p.sex);                                                                     //genero
        dprintf(fd_controlo_g, "%-6d - ", p.duration);                                                              //duração
        dprintf(fd_controlo_g, "%-10s\n", "PEDIDO");     //tipo
        sem_post(&semaforo_controlo);                                                           
    }

    return;
}

void *thread_geradora(void *argument)
{
    int i, randno;
    for (i = numero_gerador; i > 0; i--)
    {
        //gera pedido random
        randno = rand();
        PEDIDO pedido_curr;
        pedido_curr.serial_num = i;
        pedido_curr.sex = (randno % 2) ? 'M' : 'F';
        pedido_curr.rejeicoes = 0;
        pedido_curr.duration = (randno % numero_max_tempo) + 1;
        //envia pedido random
        envia_pedido(pedido_curr);
    }
    return NULL;
}

//argv[1]-numero de pedidos a enviar
//argv[2]-tempo max. de utilização (ms)
int main(int argc, char *argv[])
{
    //seed para os rands
    srand(time(NULL));
    //relogio inicial
    

    //verifica argumentos entrada
    if (argc != 3)
    {
        printf("Command usage: gerador <no. requests> <max time>\n");
        exit(1);
    }
    //criar fifos
    if (fifo_init() < 0)
    {
        printf("GERADOR: erro ao  inicializar fifos.\n");
        exit(1);
    }
    //abre canal entrada
    fd_fifo_entrada = open(FIFO_ENTRADA, O_WRONLY | O_TRUNC);
    if (fd_fifo_entrada < 0)
    {
        printf("GERADOR: erro ao abrir fifo entrada.\n");
        exit(10);
    }
    //abre canal rejeitados
    fd_fifo_rejeitados = open(FIFO_REJEITADOS, O_RDONLY | O_TRUNC);
    if (fd_fifo_rejeitados < 0)
    {
        printf("GERADOR: erro ao abrir fifo rejeitados.\n");
        exit(10);
    }

    //pedidos a gerar
    numero_gerador = (unsigned int)atoi(argv[1]);
    //maximo de tempo para cada pedido, milisegundos
    numero_max_tempo = (unsigned int)atoi(argv[2]);

    //prepara ficheiro controlo
    sprintf(nome_ficheiro_controlo, "%s%d", SUFIXO_CONTROLO_G, getpid());
    fd_controlo_g = open(nome_ficheiro_controlo, O_WRONLY | O_TRUNC | O_CREAT, PERMISSOES_MODE);

    sem_init(&semaforo_controlo, 0, 1);

    clock_gettime(CLOCK_MONOTONIC_RAW, &time_init_g);

    pthread_t tid_gera, tid_rej;
    pthread_create(&tid_gera, NULL, thread_geradora, NULL);
    pthread_create(&tid_rej, NULL, thread_trata_rejeitados, NULL);

    pthread_join(tid_gera, NULL);
    pthread_join(tid_rej, NULL);

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

    //fechar ficheiro de controlo
    if (close(fd_controlo_g) < 0)
    {
        perror("GERADOR: erro ao fechar ficheiro de controlo");
    }
    //estatisticas
    dprintf(STDOUT_FILENO, "ESTATISTICAS:\n");
    dprintf(STDOUT_FILENO, "Pedidos gerados:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n", estat_gerados_m, estat_gerados_f, estat_gerados_m + estat_gerados_f);
    dprintf(STDOUT_FILENO, "Pedidos rejeitados:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n", estat_rejeitados_m, estat_rejeitados_f, estat_rejeitados_m + estat_rejeitados_f);
    dprintf(STDOUT_FILENO, "Pedidos descartados:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n", estat_descartados_m, estat_descartados_f, estat_descartados_m + estat_descartados_f);
    //destruir fifos
    if (fifo_destroy() < 0)
    {
        printf("GERADOR: erro ao apagar fifos.\n");
        exit(1);
    }
    return 0;
}
