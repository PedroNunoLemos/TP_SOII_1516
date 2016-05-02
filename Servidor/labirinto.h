#pragma once

#include "celula.h"

#define MAXTAMX 100
#define MAXTAMY 100

typedef struct {
	
	int x;
	int y;
	int w;
	int h;
	
	
} Sala;

typedef struct 
{
	int tamx;
	int tamy;

	Celula celula[MAXTAMX][MAXTAMY];
	Sala salas[20]; 
	
} Labirinto;


//metodos do labirinto

//Cria Labirinto
Labirinto CriaLabirinto(Labirinto lab, int tamx, int tamy);
// cria uma sala no labirinto e devolve o labirinto alterado
Labirinto  CriaSala(int inix, int iniy,  Labirinto lab,int seed);

