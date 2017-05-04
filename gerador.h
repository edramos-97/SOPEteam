#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Utils.h"

char SUFIXO_CONTROLO_G[] = "/tmp/ger.";
int fd_controlo_g;

unsigned int contador = 1;

unsigned int numero_gerador;
unsigned int numero_max_tempo;