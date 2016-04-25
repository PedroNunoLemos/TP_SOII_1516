#pragma once

#include "celula.h"

#define MAXTAMX 100
#define MAXTAMY 100

typedef struct 
{
	int tamx;
	int tamy;
	Celula celulas[MAXTAMX][MAXTAMY];

} Labirinto;