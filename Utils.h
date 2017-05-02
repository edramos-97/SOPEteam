#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define OK 0
#define NOT_OK -1
#define TIPO_FICHEIRO 'f'
#define TIPO_DIRETORIO 'd'
#define TIPO_LIGACAO 'l'
#define TIPO_NULL 'X'
#define DEAD 0
#define ALIVE 100
#define NAME__MAX 255

//ESTRUTURA CONFIGURACAO
typedef struct {
	//dar print ou nao
	int go_print;
	//nome ficheiro/diretorio a procurar
	int go_name;
	char name[NAME__MAX];
	//apagar ficheiro/diretorio ou nao
	int go_delete;
	//executar comando ou nao
	int go_exec;
	char command[NAME__MAX];
	//tipo de ficheiro a procurar
	char go_type;
	//permissoes em octal
	int go_permissions;
} CONFIG;

typedef struct{
	//numero de serie do pedido
	int p;

	//sexo da pessoa ('F' ou 'M')
	char g;

	//duracao da utilizacao
	int t;

	//numero de rejeicoes (entre 0 e 3)
	int r;
} Pedido;

//le configuracao desejada
void le_config(int argc, char *argv[], CONFIG * configuracao);

void printConfig(CONFIG c);

#endif /* UTILS_H */
