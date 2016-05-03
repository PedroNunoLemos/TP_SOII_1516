#pragma once

#include <windows.h>
#include <tchar.h>

#include "coordenada.h"
#include "jogador.h"
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


typedef struct  _jogo
{
	Jogador jogadores[9];
	Celula mapaAtual[20][20];

} Jogo;

//extern DLL_IMP_API Jogo Juego;

