#pragma once


#include "../Controlador/celula.h"
#include "../Controlador/uteis.h"
#include "../Controlador/constantes.h"


typedef struct _sala{

	int x;
	int y;
	int w;
	int h;

	int salaLigada[4];
	int id;

	Coordenada porta;

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
Labirinto *CriaLabirinto(int tamx, int tamy, int salas);

int validaMovimentoBase(Labirinto lab, int x, int y);



