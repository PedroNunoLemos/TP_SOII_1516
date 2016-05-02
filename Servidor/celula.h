#pragma once


#include "coordenada.h"

typedef enum {

	//Celula representa espaço vazio no labirinto
	TipoCelula_VAZIO,

	//Celula representa uma parede no labirinto
	TipoCelula_PAREDE,

	//Celula repesenta uma porta
	TipoCelula_PORTA,

	//Celula representa o chao
	TipoCelula_CHAO,

} TipoCelula;

typedef struct
{
	TipoCelula tipo;
	Coordenada ponto;

} Celula;
