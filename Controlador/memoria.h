#pragma once
#include <windows.h>
#include <tchar.h>

#include "coordenada.h"
#include "constantes.h"
#include "jogador.h"
#include "objecto.h"
#include "monstro.h"
#include "celula.h"


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


typedef struct _memoriaPartilhada {

	Jogador jogadores[MAXJOGADORES];
	Monstro monstros[MAXINIMIGOS];
	Celula mapa[MAXTAMX][MAXTAMY];

} MemoriaPartilhada;
