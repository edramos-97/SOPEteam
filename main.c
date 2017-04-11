#include <signal.h>
#include "Utils.h"
#include "Processamento.h"

//VARIAVEIS GLOBAIS
int father_is_dead;
pid_t pid_father;

//handler para o CTRL-C
void sigint_handler(int signo) {
	if (getpid() != pid_father) {
		return;
	}

	char input;
	printf("Entered SIGINT handler ...\n");
	printf("Are you sure you want to terminate (Y/N)?\n");
	scanf("%c", &input);
	switch (input) {
	case 'y':
		printf("Process terminated!\n");
		father_is_dead = DEAD;
		break;
	case 'Y':
		printf("Process terminated!\n");
		father_is_dead = DEAD;
		break;
	case 'n':
		printf("Process resumed.\n");
		break;
	case 'N':
		printf("Process resumed.\n");
		break;
	}
	return;
}

//-name string -perm octal -type c -print -delete -exec 
int main(int argc, char *argv[]) {
	//qualquer programa tem pelo menos 5 argumentos
	if (argc < 5) {
		printf("Wrong usage of the program! %d terminating!\n", getpid());
		exit(1);
	}

	if (signal(SIGINT, sigint_handler) < 0) {
		printf("Unable to install SIGINT handler\n");
		exit(1);
	}
	printf("Process %d initiating...\n", getpid());

	//inicializa variaveis globais
	father_is_dead = ALIVE;
	pid_father = getpid();

	//le mode de operacao programa
	CONFIG configuracao;
	le_config(argc, argv, &configuracao);

	//tenta abrir diretorio escolhido pelo utilizador
	//char * caminho_escolhido = argv[1];
	DIR * diretorio_escolhido = opendir(argv[1]);
	if (diretorio_escolhido == NULL) {
		printf("ERROR OPENING DIRECTORY!\n");
		exit(1);
	}
	//chama funcao recursiva que processa tudo
	subdirectory_atomic_analyzer(diretorio_escolhido, configuracao,
			&father_is_dead, &pid_father);

	printf("END of MAIN, process %d!\n", getpid());
	return 0;
}
