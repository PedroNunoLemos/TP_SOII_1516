#pragma once

#include "objectos.h"
#include "jogadores.h"
#include "monstros.h"
#include "coordenada.h"

typedef enum tipocelula
{
	VAZIO,
	PAREDE,
	PAREDE_PO,
	PORTA,
	CHAO_PEDRA,
	CHAO_PO,
	SAIDANIVEL
	
} TipoCelula;

typedef struct celula
{

        Coordenada coordenadas;
	Jogador jogador;
	Item objecto;
	TipoCelula tipo;
	
} Celula;
