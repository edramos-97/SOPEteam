#include "Utils.h"

//le configuracao desejada
void le_config(int argc, char *argv[], CONFIG * configuracao) {
	configuracao->go_delete = NOT_OK;
	configuracao->go_exec = NOT_OK;
	configuracao->go_name = NOT_OK;
	configuracao->go_permissions = NOT_OK;
	configuracao->go_print = NOT_OK;
	configuracao->go_type = TIPO_NULL;
	int i;
	for(i =0 ; i < argc; i++){
		if(strcmp(argv[i],"-name")==OK){
			configuracao->go_name = OK;
			stpcpy(configuracao->name,argv[i+1]);
		}
		if(strcmp(argv[i],"-exec")==OK){
			configuracao->go_exec = OK;
			stpcpy(configuracao->command,argv[i+1]);
		}
		if(strcmp(argv[i],"-print")==OK){
			configuracao->go_print = OK;
		}
		if(strcmp(argv[i],"-delete")==OK){
			configuracao->go_delete = OK;
		}
		if(strcmp(argv[i],"-type")==OK){
			configuracao->go_type = (argv[i+1])[0];
		}
		if(strcmp(argv[i],"-perm")==OK){
			configuracao->go_permissions = strtol(argv[i+1], NULL, 8);

		}
	}
}


void printConfig(CONFIG c){
	if(c.go_print == OK)
		printf("PRINT: ON\n");
	else printf("PRINT:OFF\n");

	if(c.go_name == OK){
		printf("NAME: ON\n");
		printf("%s!\n",c.name);
	}
	else printf("NAME: OFF\n");

	if(c.go_delete == OK)
		printf("DELETE: ON\n");
	else printf("DELETE: OFF\n");

	if(c.go_exec == OK){
		printf("EXEC: ON\n");
		printf("%s!\n", c.command);
	}
	else printf("EXEC: OFF\n");

	if(c.go_type == TIPO_NULL)
		printf("TYPE: OFF\n");
	else printf("TYPE: ON\n%c!\n",c.go_type);

	if(c.go_permissions == NOT_OK)
		printf("PERM: OFF\n");
	else printf("PERM: ON\n%o!\n",c.go_permissions);
}




