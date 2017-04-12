#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Utils.h"

void subdirectory_atomic_analyzer(char * atual, CONFIG configuracao,
		int * global_father, pid_t * pid_father);

#endif /* PROCESSAMENTO_H */
