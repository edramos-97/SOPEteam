#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Utils.h"

char SUFIXO_CONTROLO_S[] = "/tmp/bal.";
int fd_controlo_s;
char nome_ficheiro_controlo[sizeof(SUFIXO_CONTROLO_S)+10]; //pid tem no maximo 5 digitos, mais um para nulo terminador

unsigned int numero_vagas;

//nr pedidos recebidos,rejeicoes ,servidos
unsigned int estat_recebidos_m = 0;
unsigned int estat_recebidos_f = 0;
unsigned int estat_rejeitados_m = 0;
unsigned int estat_rejeitados_f = 0;
unsigned int estat_servidos_m = 0;
unsigned int estat_servidos_f = 0;