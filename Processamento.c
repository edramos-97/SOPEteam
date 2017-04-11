#include "Processamento.h"

void subdirectory_atomic_analyzer(DIR * atual, CONFIG configuracao,
		int * global_father, pid_t * pid_father) {

	if((*(global_father)) == DEAD)
		return;


	struct dirent * directorio;
	struct stat estado_ficheiro;

	//percorre ficheiros
	if(configuracao.go_type != TIPO_DIRETORIO){
		while((struct dirent *)(directorio = readdir(atual)) != NULL){
			//verificar os ficheiros conforme config




		}

		//faz rewind para na proxima volta ler os diretorios.
		rewinddir(atual);
	}

	//percorre diretorios (onde vai ter o fork)
	while((struct dirent *)(directorio = readdir(atual)) != NULL){
		//verificar subdiretorios chamando recursivamente esta funcao



	}

	if(getpid() == (*(pid_father)))
		wait();

	return;
}


//-//	DIR *dirp;
// -//	struct dirent *direntp;
// -//	struct stat stat_buf;
// -//	char *str;
// -//	char name[200];
// -//	if (argc != 2) {
// -//		fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
// -//		exit(1);
// -//	}
// -//	if ((dirp = opendir(argv[1])) == NULL) {
// -//		perror(argv[1]);
// -//		exit(2);
// -//	}
// -//	while ((direntp = readdir(dirp)) != NULL) {
// -//		sprintf(name, "%s/%s", argv[1], direntp->d_name); // <----- NOTAR
// -//		// alternativa a chdir(); ex: anterior
// -//		if (lstat(name, &stat_buf) == -1) // testar com stat()
// -//				{
// -//			perror("lstat ERROR");
// -//			exit(3);
// -//		}
// -//		// printf("%10d - ",(int) stat_buf.st_ino);
// -//		if (S_ISREG(stat_buf.st_mode))
// -//			str = "regular";
// -//		else if (S_ISDIR(stat_buf.st_mode))
// -//			str = "directory";
// -//		else
// -//			str = "other";
// -//		printf("%-25s - %s\n", direntp->d_name, str);
// -//	}
// -//	closedir(dirp);
// -//	exit(0);
