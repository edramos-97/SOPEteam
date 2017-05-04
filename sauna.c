#include "sauna.h"


void * thread_principal(void * argumento){ //le da entrada
    do{
        PEDIDO pedido_recebido;
        read(fd_fifo_entrada,&pedido_recebido,sizeof(pedido_recebido));
        //atualiza estatistica + controlo
        if(flag_recebeu == NAO_RECEBEU_PRIMEIRO){
            flag_recebeu = RECEBEU_PRIMEIRO;
            pedidos_restantes = pedido_recebido.serial_num;
        }

        sem_wait(&acesso_var_livres);
        if(estado_sauna == VAZIA){
            estado_sauna = pedido_recebido.sex;
            lugares_livres--;
            sem_post(&acesso_var_livres);

             //pedido é SERVIDO
             pedidos_restantes--; //vai ser atendido, menos um pa preocupar
             //atualiza estatistica + controlo
             sem_wait(&semaforo_vagas); //menos uma vaga
            //criar thread de espera
        }
        else if(estado_sauna != pedido_recebido.sex){ //rejeitar
            //atualiza estatistica + controlo
            if(pedido_recebido.rejeicoes == 2) //sera descartado seguidamente entao menos um pa preocupar
                pedidos_restantes--;
            
        }
        else { //valores iguais 
            //atualiza estatistica + controlo
            pedidos_restantes--; //vai ser atendido, menos um pa preocupar
            sem_wait(&semaforo_vagas); //menos uma vaga pa meter aqui
            //criar thread de espera
        }

        //ler pedido

        //verificar sexo compativel s/n

        //n -> rejeitar e verificar se é descartavel para decrementar pedidos_restantes

        //s -> fazer wait do semaforo e se servir decrementar pedidos restantes/lançar threads filhos

        
    }while(pedidos_restantes > 0);
    //fechar fifo rejeitados
    //esperar threads filhas


    //estatisticas
    dprintf(STDOUT_FILENO,"ESTATISTICAS:\n");
    dprintf(STDOUT_FILENO,"Pedidos recebidos:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n",estat_recebidos_m,estat_recebidos_f,estat_recebidos_m+estat_recebidos_f);
    dprintf(STDOUT_FILENO,"Pedidos rejeitados:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n",estat_rejeitados_m,estat_rejeitados_f,estat_rejeitados_m+estat_rejeitados_f);
    dprintf(STDOUT_FILENO,"Pedidos servidos:\n%10d Homens;\n%10d Mulheres;\nTotais:%10d\n",estat_servidos_m,estat_servidos_f,estat_servidos_m+estat_servidos_f);
    
    return NULL;
}


void * thread_espera(void * argumento){ //argumento sera o tempo a esperar
    usleep((*((int*)argumento))*1000);
    free(argumento);
    
    sem_wait(&acesso_var_livres);
    lugares_livres++;
    if(lugares_livres == numero_vagas)
        estado_sauna = VAZIA;
    sem_post(&acesso_var_livres);
    sem_post(&semaforo_vagas);
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

    sem_init(&semaforo_vagas,0,numero_vagas);
    sem_init(&acesso_var_livres,0,1);



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


