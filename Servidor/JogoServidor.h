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

	int JogoIniciado;
	int ClienteLigado;
	int jogoVisivel;

	HANDLE hPipeA[MAXJOGADORES];
	HANDLE pipeAtual;

	DWORD totalLigacoes;

	Vitamina vitaminas[MAXVITAMINAS];
	OrangeBull oranges[MAXORANGES];
	Pedra pedras[MAXPEDRAS];
	Cafeina cafeinas[MAXCAFEINAS];

} JogoServidor;

