#ifndef UTILS
#define UTILS
#define OK 0
#define TIPO_FICHEIRO 'f'
#define TIPO_DIRETORIO 'd'
#define TIPO_LIGACAO 'l'
#define DEAD 0
#define ALIVE 100
#define NAME_MAX 255

//ESTRUTURA CONFIGURACAO
struct CONFIG {
	//dar print ou nao
	int go_print;
	//nome ficheiro/diretorio a procurar
	int go_name;
	char name_print[NAME_MAX];
	//apagar ficheiro/diretorio ou nao
	int go_delete;
	//executar comando ou nao
	int go_exec;
	//tipo de ficheiro a procurar
	char go_tipe;
	//permissoes em octal
	int go_permissions;
};

#endif
