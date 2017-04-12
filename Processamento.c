#include "Processamento.h"

void subdirectory_atomic_analyzer(char * atual, CONFIG configuracao,
		int * global_father, pid_t * pid_father) {

	if ((*(global_father)) == DEAD)
		return;

	//tenta abrir diretorio escolhido
	DIR * diretorio_escolhido = opendir(atual);
	if (diretorio_escolhido == NULL) {
		printf("ERROR OPENING DIRECTORY!\n");
		exit(1);
	}

	struct dirent * ficheiro;
	struct stat estado_ficheiro;

	//percorre ficheiros
	if (configuracao.go_type != TIPO_DIRETORIO) {
		while ((struct dirent *) (ficheiro = readdir(diretorio_escolhido))
				!= NULL) {

			if ((*(global_father)) == DEAD)
				break;
			//verificar os ficheiros conforme config
			char ficheiro_atual[NAME__MAX];
			sprintf(ficheiro_atual, "%s/%s", atual, ficheiro->d_name);
			if (stat(ficheiro_atual, &estado_ficheiro) == NOT_OK)
				printf("There was an error trying to get the state of %s.",
						ficheiro->d_name);
			else {
				if (configuracao.go_type == TIPO_NULL) { //nao especificou tipo

					if (configuracao.go_name == OK) { //especificou nome
						if (strcmp(configuracao.name, ficheiro->d_name) == OK) {
							if (configuracao.go_print == OK
									&& configuracao.go_exec != OK) {
								printf("%s\n", ficheiro_atual); //imprime ficheiro
							}

							if (configuracao.go_exec == OK) { //executa comando
								char comando[NAME__MAX];
								sprintf(comando, "%s %s", configuracao.command,
										ficheiro_atual);
								system(comando);
							}

							if (configuracao.go_delete == OK) {
								char comando[NAME__MAX];
								sprintf(comando, "rm -f %s", ficheiro_atual); //apaga
								system(comando);
							}
						} else
							continue; //nome diferente, passa à frente

					} else { // nao especificou nome, verificar permissoes
						if (configuracao.go_permissions != NOT_OK) { //especificou permissoes
							int mode = estado_ficheiro.st_mode
									& (S_IRWXU | S_IRWXG | S_IRWXO);

							if (mode == configuracao.go_permissions) { //mesmas permissoes
								if (configuracao.go_print == OK
										&& configuracao.go_exec != OK) {
									printf("%s\n", ficheiro_atual); //imprime ficheiro
								}

								if (configuracao.go_exec == OK) { //executa comando
									char comando[NAME__MAX];
									sprintf(comando, "%s %s",
											configuracao.command,
											ficheiro_atual);
									system(comando);
								}

								if (configuracao.go_delete == OK) {
									char comando[NAME__MAX];
									sprintf(comando, "rm -f %s",
											ficheiro_atual); //apaga
									system(comando);
								}

							} else
								continue;

						} else { //nao especificou permissoes
							if (configuracao.go_print == OK
									&& configuracao.go_exec != OK) {
								printf("%s\n", ficheiro_atual); //imprime ficheiro
							}

							if (configuracao.go_exec == OK) { //executa comando
								char comando[NAME__MAX];
								sprintf(comando, "%s %s", configuracao.command,
										ficheiro_atual);
								system(comando);
							}

							if (configuracao.go_delete == OK) {
								char comando[NAME__MAX];
								sprintf(comando, "rm -f %s", ficheiro_atual); //apaga
								system(comando);
							}

						}

					}

				} else {	//se o tipo corresponde ao tipo do ficherio
					if ((configuracao.go_type == TIPO_FICHEIRO
							&& S_ISREG(estado_ficheiro.st_mode))
							|| (configuracao.go_type == TIPO_LIGACAO
									&& S_ISLNK(estado_ficheiro.st_mode))) {

						if (configuracao.go_name == OK) { //especificou nome
							if (strcmp(configuracao.name,
									ficheiro->d_name) == OK) {
								if (configuracao.go_print == OK
										&& configuracao.go_exec != OK) {
									printf("%s\n", ficheiro_atual); //imprime ficheiro
								}

								if (configuracao.go_exec == OK) { //executa comando
									char comando[NAME__MAX];
									sprintf(comando, "%s %s",
											configuracao.command,
											ficheiro_atual);
									system(comando);
								}

								if (configuracao.go_delete == OK) {
									char comando[NAME__MAX];
									sprintf(comando, "rm -f %s",
											ficheiro_atual); //apaga
									system(comando);
								}
							} else
								continue; //nome diferente, passa à frente

						} else { // nao especificou nome, verificar permissoes
							if (configuracao.go_permissions != NOT_OK) { //especificou permissoes
								int mode = estado_ficheiro.st_mode
										& (S_IRWXU | S_IRWXG | S_IRWXO);

								if (mode == configuracao.go_permissions) { //mesmas permissoes
									if (configuracao.go_print == OK
											&& configuracao.go_exec != OK) {
										printf("%s\n", ficheiro_atual); //imprime ficheiro
									}

									if (configuracao.go_exec == OK) { //executa comando
										char comando[NAME__MAX];
										sprintf(comando, "%s %s",
												configuracao.command,
												ficheiro_atual);
										system(comando);
									}

									if (configuracao.go_delete == OK) {
										char comando[NAME__MAX];
										sprintf(comando, "rm -f %s",
												ficheiro_atual); //apaga
										system(comando);
									}

								} else
									continue;

							} else { //nao especificou permissoes
								if (configuracao.go_print == OK
										&& configuracao.go_exec != OK) {
									printf("%s\n", ficheiro_atual); //imprime ficheiro
								}

								if (configuracao.go_exec == OK) { //executa comando
									char comando[NAME__MAX];
									sprintf(comando, "%s %s",
											configuracao.command,
											ficheiro_atual);
									system(comando);
								}

								if (configuracao.go_delete == OK) {
									char comando[NAME__MAX];
									sprintf(comando, "rm -f %s",
											ficheiro_atual); //apaga
									system(comando);
								}

							}

						}

					}
				}
			}
		}

		//faz rewind para na proxima volta ler os diretorios.
		rewinddir(diretorio_escolhido);
	}


	//percorre diretorios (onde vai ter o fork)
	while ((struct dirent *) (ficheiro = readdir(diretorio_escolhido)) != NULL) {
		if ((*(global_father)) == DEAD)
			break;
		//verificar subdiretorios chamando recursivamente esta funcao
		char ficheiro_atual[NAME__MAX];
		sprintf(ficheiro_atual, "%s/%s", atual, ficheiro->d_name);
		if (stat(ficheiro_atual, &estado_ficheiro) == NOT_OK)
			printf("There was an error trying to get the state of %s.",
					ficheiro->d_name);
		if (S_ISDIR(estado_ficheiro.st_mode) && strcmp(".",ficheiro->d_name) != OK && strcmp("..",ficheiro->d_name) != OK) { //usa fork
			if (fork() == 0) { //FILHO
				subdirectory_atomic_analyzer(ficheiro_atual, configuracao,
						global_father, pid_father);
			}
		}
	}

	while (wait(NULL) != NOT_OK);
	if(getpid() != (*pid_father))
		exit(0);

	return;
}
