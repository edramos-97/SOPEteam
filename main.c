#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>




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

//VARIAVEIS GLOBAIS
pid_t father_is_dead;
int pid_father;

//le configuracao desejada
void le_config(int argc, char *argv[], struct CONFIG * configuracao) {

}

//handler para o CTRL-C
void sigint_handler(int signo) {
	if(getpid() != pid_father){
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
	
	fork();
	sleep(5);
	
//	DIR *dirp;
//	struct dirent *direntp;
//	struct stat stat_buf;
//	char *str;
//	char name[200];
//	if (argc != 2) {
//		fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
//		exit(1);
//	}
//	if ((dirp = opendir(argv[1])) == NULL) {
//		perror(argv[1]);
//		exit(2);
//	}
//	while ((direntp = readdir(dirp)) != NULL) {
//		sprintf(name, "%s/%s", argv[1], direntp->d_name); // <----- NOTAR
//		// alternativa a chdir(); ex: anterior
//		if (lstat(name, &stat_buf) == -1) // testar com stat()
//				{
//			perror("lstat ERROR");
//			exit(3);
//		}
//		// printf("%10d - ",(int) stat_buf.st_ino);
//		if (S_ISREG(stat_buf.st_mode))
//			str = "regular";
//		else if (S_ISDIR(stat_buf.st_mode))
//			str = "directory";
//		else
//			str = "other";
//		printf("%-25s - %s\n", direntp->d_name, str);
//	}
//	closedir(dirp);
//	exit(0);

	
	printf("END of process %d and father is %d!\n", getpid(),father_is_dead);
	return 0;
}
