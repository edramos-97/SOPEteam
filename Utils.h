#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

typedef struct{
	//numero de serie do pedido
	unsigned int serial_num;

	//sexo da pessoa ('F' ou 'M')
	char sex;

	//duracao da utilizacao
	int duration;

	//numero de rejeicoes (entre 0 e 3)
	int rejections;
} PEDIDO;

char FIFO_ENTRADA[] = "/tmp/entrada";
char FIFO_REJEITADOS[] = "/tmp/rejeitados";


#endif /* UTILS_H */
