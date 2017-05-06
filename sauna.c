#include "sauna.h"

void *thread_principal(void *argumento)
{ //le da entrada
    do
    {
        PEDIDO pedido_recebido;
        read(fd_fifo_entrada, &pedido_recebido, sizeof(pedido_recebido));

        //atualiza estatistica + controlo
        if (pedido_recebido.sex == 'F')
            estat_recebidos_f++;
        else
            estat_recebidos_m++;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_curr);
        dprintf(fd_controlo_s, "%.2fms - ", convertToMilliseconds(time_curr) - convertToMilliseconds(time_init)); //tempo
        dprintf(fd_controlo_s, "%-5d - ", getpid());   //pid proc
        dprintf(fd_controlo_s, "%-5lu - ", pthread_self()); //tid
        dprintf(fd_controlo_s, "%-10u: ", pedido_recebido.serial_num); //num pedido
        dprintf(fd_controlo_s, "%c - ", pedido_recebido.sex);          //genero
        dprintf(fd_controlo_s, "%-6d - ", pedido_recebido.duration);   //duração
        dprintf(fd_controlo_s, "%-10s\n", "RECEBIDO");   //tipo

        //print_info(pedido_recebido,"RECEBIDO");

        if (flag_recebeu == NAO_RECEBEU_PRIMEIRO)
        {
            flag_recebeu = RECEBEU_PRIMEIRO;
            pedidos_restantes = pedido_recebido.serial_num;
        }

        sem_wait(&acesso_var_livres);
        if (estado_sauna == VAZIA)
        {
            estado_sauna = pedido_recebido.sex;
            lugares_livres--;
            sem_post(&acesso_var_livres);

            //pedido é SERVIDO
             pedidos_restantes--;       //vai ser atendido, menos um pa preocupar
            //atualiza estatistica + controlo
            if (pedido_recebido.sex == 'F')
                estat_servidos_f++;
            else
                estat_servidos_m++;
            clock_gettime(CLOCK_MONOTONIC_RAW, &time_curr);
            dprintf(fd_controlo_s, "%.2fms - ", convertToMilliseconds(time_curr) - convertToMilliseconds(time_init)); //tempo
            dprintf(fd_controlo_s, "%-5d - ", getpid());   //pid proc
            dprintf(fd_controlo_s, "%-5lu - ", pthread_self()); //tid
            dprintf(fd_controlo_s, "%-10u: ", pedido_recebido.serial_num); //num pedido
            dprintf(fd_controlo_s, "%c - ", pedido_recebido.sex);          //genero
            dprintf(fd_controlo_s, "%-6d - ", pedido_recebido.duration);   //duração
            dprintf(fd_controlo_s, "%-10s\n", "SERVIDO");   //tipo

            
            sem_wait(&semaforo_vagas); //menos uma vaga
            //criar thread de espera
            pthread_t tid;
            int * tempo = malloc(sizeof(int));
            (*tempo) = pedido_recebido.duration;
            pthread_create(&tid,NULL,thread_espera,(void *) tempo);
        }
        else if (estado_sauna != pedido_recebido.sex)
        { //rejeitar
            sem_post(&acesso_var_livres);
            if (pedido_recebido.rejeicoes == 2) //sera descartado seguidamente entao menos um pa preocupar
                pedidos_restantes--;

            //atualiza estatistica + controlo
            if (pedido_recebido.sex == 'F')
                estat_rejeitados_f++;
            else
                estat_rejeitados_f++;
            clock_gettime(CLOCK_MONOTONIC_RAW, &time_curr);
            dprintf(fd_controlo_s, "%.2fms - ", convertToMilliseconds(time_curr) - convertToMilliseconds(time_init)); //tempo
            dprintf(fd_controlo_s, "%-5d - ", getpid());    //pid proc
            dprintf(fd_controlo_s, "%-5lu - ", pthread_self()); //tid
            dprintf(fd_controlo_s, "%-10u: ", pedido_recebido.serial_num); //num pedido
            dprintf(fd_controlo_s, "%c - ", pedido_recebido.sex);          //genero
            dprintf(fd_controlo_s, "%-6d - ", pedido_recebido.duration);   //duração
            dprintf(fd_controlo_s, "%-10s\n", "REJEITADO");   //tipo

            //escrever no fifo rejeitados

            write(fd_fifo_rejeitados,&pedido_recebido,sizeof(pedido_recebido));
        }
        else
        { //valores iguais
            lugares_livres--;
            sem_post(&acesso_var_livres);
            sem_wait(&semaforo_vagas); //menos uma vaga pa meter aqui

            
        

            pedidos_restantes--;       //vai ser atendido, menos um pa preocupar
             //atualiza estatistica + controlo
            if (pedido_recebido.sex == 'F')
                estat_servidos_f++;
            else
                estat_servidos_m++;
            clock_gettime(CLOCK_MONOTONIC_RAW, &time_curr);
            dprintf(fd_controlo_s, "%.2fms - ", convertToMilliseconds(time_curr) - convertToMilliseconds(time_init)); //tempo
            dprintf(fd_controlo_s, "%-5d - ", getpid());   //pid                                                           
            dprintf(fd_controlo_s, "%-5lu - ", pthread_self()); //tid
            dprintf(fd_controlo_s, "%-10u: ", pedido_recebido.serial_num); //num pedido
            dprintf(fd_controlo_s, "%c - ", pedido_recebido.sex);          //genero
            dprintf(fd_controlo_s, "%-6d - ", pedido_recebido.duration);   //duração
            dprintf(fd_controlo_s, "%-10s\n", "SERVIDO");   //tipo
            //criar thread de espera
            pthread_t tid;
            int * tempo = malloc(sizeof(int));
            (*tempo) = pedido_recebido.duration;
            pthread_create(&tid,NULL,thread_espera,(void *) tempo);
        }

    } while (pedidos_restantes > 0);

    //fechar fifo rejeitados
    close(fd_fifo_rejeitados);
    close(fd_fifo_entrada);
    //esperar threads filhas 
    while(estado_sauna != VAZIA)
        usleep(100000); //espera 100ms
    //IDEA: esperar pelas threads filhas consultando em espera ativa o valor da variavel estado_sauna.

   

    //estatisticas
    dprintf(STDOUT_FILENO, "ESTATISTICAS:\n");
    dprintf(STDOUT_FILENO, "Pedidos recebidos:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n", estat_recebidos_m, estat_recebidos_f, estat_recebidos_m + estat_recebidos_f);
    dprintf(STDOUT_FILENO, "Pedidos rejeitados:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n", estat_rejeitados_m, estat_rejeitados_f, estat_rejeitados_m + estat_rejeitados_f);
    dprintf(STDOUT_FILENO, "Pedidos servidos:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n", estat_servidos_m, estat_servidos_f, estat_servidos_m + estat_servidos_f);

    return NULL;
}

void *thread_espera(void *argumento)
{ //argumento sera o tempo a esperar
    usleep((*((int *)argumento)) * 1000);
    free(argumento);

    sem_wait(&acesso_var_livres);
    lugares_livres++;
    if (lugares_livres == numero_vagas)
        estado_sauna = VAZIA;
    sem_post(&acesso_var_livres);
    sem_post(&semaforo_vagas);
    return NULL;
}

void print_info(PEDIDO p,char* tipo){
    dprintf(fd_controlo_s, "%10.2fms - ", convertToMilliseconds(time_curr) - convertToMilliseconds(time_init)); //tempo
    dprintf(fd_controlo_s, "%-5d - ", getpid());                                                                //pid proc
    dprintf(fd_controlo_s, "%-10u: ", p.serial_num);                                                            //num pedidos
    dprintf(fd_controlo_s, "%c - ", p.sex);                                                                     //genero
    dprintf(fd_controlo_s, "%-6d - ", p.duration);                                                              //duração
    dprintf(fd_controlo_s, "%-10s\n", tipo);
}

//argv[1]-numero de pedidos a enviar
int main(int argc, char *argv[])
{
    

    //verifica argumentos entrada
    if (argc != 2)
    {
        printf("Command usage: gerador <no. seats>\n");
        exit(1);
    }

    //criar fifos
    if (fifo_init() < 0)
    {
        printf("SAUNA: erro ao  inicializar fifos.\n");
        exit(1);
    }

    //abre canal de entrada
    fd_fifo_entrada = open(FIFO_ENTRADA, O_RDONLY | O_TRUNC);
    if (fd_fifo_entrada < 0)
    {
        printf("SAUNA: erro ao abrir fifo entrada.\n");
        exit(10);
    }
    //abre cana rejeitados
    fd_fifo_rejeitados = open(FIFO_REJEITADOS, O_WRONLY | O_TRUNC);
    if (fd_fifo_rejeitados < 0)
    {
        printf("SAUNA: erro ao abrir fifo rejeitados.\n");
        exit(10);
    }

    //le numero vagas da sauna e atualiza os livres
    numero_vagas = (unsigned int)atoi(argv[1]);
    lugares_livres = numero_vagas;

    //prepara ficheiro controlo
    sprintf(nome_ficheiro_controlo, "%s%d", SUFIXO_CONTROLO_S, getpid());
    fd_controlo_s = open(nome_ficheiro_controlo, O_WRONLY | O_TRUNC | O_CREAT, PERMISSOES_MODE);

    sem_init(&semaforo_vagas, 0, numero_vagas);
    sem_init(&acesso_var_livres, 0, 1);

    //relogio inicial
    clock_gettime(CLOCK_MONOTONIC_RAW, &time_init);

    pthread_t tid;
    pthread_create(&tid,NULL,thread_principal,NULL);
    pthread_join(tid,NULL);

    //fechar ficheiro de controlo + estatisticas
    if (close(fd_controlo_s) < 0)
    {
        perror("SAUNA: erro ao fechar ficheiro de controlo");
    }
    //destruir fifos
    if (fifo_destroy() < 0)
    {
        printf("SAUNA: erro ao apagar fifos.\n");
        exit(1);
    }
    return 0;
}
