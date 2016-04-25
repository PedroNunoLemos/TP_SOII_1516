#pragma once

typedef enum tipomonstro
{
	DISTRAIDO,
	BULLY

}TipoMonstro;

typedef struct monstro
{

	int lentidao;
	TipoMonstro tipo;

	int energia;

	char instancia;//corrigir para handle

} Monstro;