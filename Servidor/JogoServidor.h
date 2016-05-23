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
	PosicaoJogadorMapa jogadoresMapa[MAXTAMX][MAXTAMX];

	JogoCliente clientes[MAXJOGADORES];

	int JogoIniciado;
	int ClienteLigado;
	int jogoVisivel;



	DWORD totalLigacoes;


} JogoServidor;

