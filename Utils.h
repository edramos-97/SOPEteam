#ifndef UTILS
#define UTILS

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define OK 0
#define NOT_OK -1
#define TIPO_FICHEIRO 'f'
#define TIPO_DIRETORIO 'd'
#define TIPO_LIGACAO 'l'
#define TIPO_NULL 'X'
#define DEAD 0
#define ALIVE 100
#define NAME_MAX 255

//ESTRUTURA CONFIGURACAO
typedef struct {
	//dar print ou nao
	int go_print;
	//nome ficheiro/diretorio a procurar
	int go_name;
	char name[NAME_MAX];
	//apagar ficheiro/diretorio ou nao
	int go_delete;
	//executar comando ou nao
	int go_exec;
	char command[NAME_MAX];
	//tipo de ficheiro a procurar
	char go_type;
	//permissoes em octal
	int go_permissions;
} CONFIG ;

//le configuracao desejada
void le_config(int argc, char *argv[], CONFIG * configuracao);

void printConfig(CONFIG c);

#endif
