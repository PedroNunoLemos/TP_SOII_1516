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


//metodos do labirinto

// cria uma sala no labirinto e devolve o labirinto alterado
Labirinto  CriaSala(int inix, int iniy, int tamx, int tamy, Labirinto lab);