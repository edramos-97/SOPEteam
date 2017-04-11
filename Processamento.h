#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include "Utils.h"

void subdirectory_atomic_analyzer(DIR * atual, CONFIG configuracao,
		int * global_father);

#endif /* PROCESSAMENTO_H */
