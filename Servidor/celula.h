#pragma once

#include "objectos.h"
#include "jogadores.h"
#include "monstros.h"
//s

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

	Jogador jogador;
	Item objecto;
	TipoCelula tipo;
	
} Celula;