#pragma once

#include <windows.h>
#include <tchar.h>

#include "coordenada.h"
#include "jogador.h"
#include "celula.h"
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


typedef struct DLL_IMP_API _posicao_JogadorMapa
{
	DWORD cor;
	DWORD pidCliente;
	DWORD posicaoOcupada;

}PosicaoJogadorMapa;

typedef struct DLL_IMP_API _jogoCliente
{

	int comando;

	int respostaComando;
	int moveuDirecao;


	Celula mapa[MAXVISX][MAXVISY];
	Jogador jogadorAt;

	PosicaoJogadorMapa jogadoresMapa[MAXVISX][MAXVISY];
	ObjectoMapa objectosMapa[MAXVISX][MAXVISY];

	DWORD pidCliente;
	Jogador jogador;

	TCHAR mensagem[256];


} JogoCliente;


