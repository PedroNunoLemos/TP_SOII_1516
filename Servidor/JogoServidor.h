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

	HANDLE hPipeA[MAXJOGADORES];
	HANDLE pipeAtual;

	DWORD totalLigacoes;

	Vitamina vitaminas[20];
	OrangeBull oranges[15];
	Pedra pedras[50];
	Cafeina cafeinas[8];

} JogoServidor;

