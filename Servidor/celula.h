#pragma once

#include "objectos.h"
#include "jogadores.h"
#include "monstros.h"
#include "sala.h"

typedef enum tipocelula
{
	PAREDE,
	PAREDE_PO,
	PORTA,
	CHAO_PEDRA,
	CHAO_PO,
	SAIDANIVEL
	
} TipoCelula;

typedef struct celula
{
	int posx;
	int posy;

	Sala salaAssociada;
	Jogador jogador;
	Item objecto;
	TipoCelula tipo;
	
} Celula;