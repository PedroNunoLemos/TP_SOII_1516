#pragma once


#include "..\Controlador\celula.h"
#include "..\Controlador\uteis.h"


#define MAXTAMX 100
#define MAXTAMY 100
#define MAXSALAS 20


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
	int tamsalas;

	Celula celula[MAXTAMX][MAXTAMY];
	Sala salas[MAXSALAS];

} Labirinto;


//metodos do labirinto


//Cria Labirinto
Labirinto CriaLabirinto(Labirinto lab, int tamx, int tamy, int salas);


