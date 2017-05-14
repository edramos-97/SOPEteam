#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Utils.h"

char SUFIXO_CONTROLO_G[] = "/tmp/ger.";
int fd_controlo_g;
char nome_ficheiro_controlo[sizeof(SUFIXO_CONTROLO_G)+10]; //pid tem no maximo 5 digitos, mais um para nulo terminador

struct timespec time_init_g;
struct timespec time_curr_g;

unsigned int numero_gerador;
unsigned int numero_max_tempo;

//nr pedidos gerados,rejeicoes recebidas,rejeicoes descartadas
unsigned int estat_gerados_m = 0;
unsigned int estat_gerados_f = 0;
unsigned int estat_rejeitados_m = 0;
unsigned int estat_rejeitados_f = 0;
unsigned int estat_descartados_m = 0;
unsigned int estat_descartados_f = 0;

sem_t semaforo_controlo;