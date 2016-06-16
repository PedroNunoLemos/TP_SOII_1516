#pragma once

#include <windows.h>
#include <tchar.h>

#include "coordenada.h"
#include "jogador.h"
#include "celula.h"
#include "constantes.h"
#include "labirinto.h"
#include "monstro.h"

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



typedef struct utilizadorhist
{
	TCHAR nome[200];


	int derrota;
	int vitoria;
	int desistencia;

} UtilizadorHist;


typedef struct historico
{
	int totReg;
	UtilizadorHist registo[5];

} Historico;

typedef struct servidorInfo {

	Historico hist;

	int jogadoresOnline;
	Jogador jogadores[MAXJOGADORES];
	
	int jogoIniciado;

} ServidorInfo;
