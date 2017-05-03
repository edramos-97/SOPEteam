#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define PERMISSOES_MODE 0660

typedef struct
{
	//numero de serie do pedido
	unsigned int serial_num;

	//sexo da pessoa ('F' ou 'M')
	char sex;

	//duracao da utilizacao
	int duration;

	//numero de rejeicoes (entre 0 e 3)
	int rejeicoes;
} PEDIDO;

char FIFO_ENTRADA[] = "/tmp/entrada";
char FIFO_REJEITADOS[] = "/tmp/rejeitados";

int fd_fifo_entrada;
int fd_fifo_rejeitados;

int fifo_init()
{
	int error_fifos;

	error_fifos = mkfifo(FIFO_ENTRADA, PERMISSOES_MODE);
	if (error_fifos != EEXIST || error_fifos != 0)
	{
		perror("ERRO FIFO ENTRADA: ");
		return -1;
	}
	error_fifos = mkfifo(FIFO_REJEITADOS, PERMISSOES_MODE);
	if (error_fifos != EEXIST || error_fifos != 0)
	{
		perror("ERRO FIFO REJEITADOS: ");
		return -1;
	}

	return 0;
}

int fifo_destroy()
{

	return 0;
}

#endif /* UTILS_H */
