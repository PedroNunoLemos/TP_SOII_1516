#pragma once

#include "celula.h"
#include "uteis.h"
#include "constantes.h"

#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)

#else
#define DLL_IMP_API __declspec(dllimport)
#endif

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif

typedef struct _sala {

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

//Labirinto *CriaLabirinto(int tamx, int tamy, int salas);

Labirinto *CriaLabirinto(Labirinto *lab, int tamx, int tamy, int salas);

