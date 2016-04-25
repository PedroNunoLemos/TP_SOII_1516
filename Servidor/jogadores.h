#pragma once

#include  "objectos.h"

typedef struct jogador
{

	 int numero;
	 char nome[256];
	 int lentidao;
	 int saude;

	 Item objectos[99];
	 

} Jogador;
