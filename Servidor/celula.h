#pragma once


#include "coordenada.h"

typedef enum {

	//Celula representa espaço vazio no labirinto
	TipoCelula_VAZIO,

	//Parede Canto superior esquerdo 
	TipoCelula_PAREDE_SE,

	//Parede Canto superior direito 
	TipoCelula_PAREDE_SD,

	//Parede Canto inferior direito 
	TipoCelula_PAREDE_ID,

	//Parede Canto inferior esquerdo 
	TipoCelula_PAREDE_IE,


	//Parede horizontal 
	TipoCelula_PAREDE_HZ,


	//Parede vertical 
	TipoCelula_PAREDE_VT,

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
