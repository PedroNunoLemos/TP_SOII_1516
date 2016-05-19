#pragma once

#include "labirinto.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\objecto.h"
#include "..\Controlador\jogador.h"


typedef struct  _jogoservidor
{

	int jogadoresLigados;

	Labirinto mapa;

	Jogador jogadores[MAXJOGADORES];
	ObjectoMapa objectos[MAXTAMX][MAXTAMY];


	int JogoIniciado;
	int ClienteLigado;
	int jogoVisivel;

	HANDLE hPipeA[MAXJOGADORES];
	HANDLE pipeAtual;

	DWORD totalLigacoes;


} JogoServidor;

