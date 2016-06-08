#pragma once

#include "labirinto.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\objecto.h"
#include "..\Controlador\jogador.h"
#include "..\Controlador\jogo.h"

typedef struct  _cliente
{
	JogoCliente jogo;
	HANDLE ligacao;

} Cliente;

typedef struct  _jogoservidor
{

	int jogadoresLigados;

	Labirinto *mapa;


	Cliente clientes[MAXJOGADORES];

	//JogoCliente jogoClientes[MAXJOGADORES];
	//HANDLE clientes[MAXJOGADORES];

	HANDLE clientes_atualizar[MAXJOGADORES];


	int JogoIniciado;
	int ClienteLigado;



	DWORD totalLigacoes;


} JogoServidor;

