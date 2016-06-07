#pragma once
#include <windows.h>
#include <tchar.h>

#include "coordenada.h"
#include "jogador.h"
#include "objecto.h"

#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)

#else
#define DLL_IMP_API __declspec(dllimport)
#endif

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif


typedef enum tipocelula {

	//Celula representa espaço vazio no labirinto
	TipoCelula_VAZIO,

	//Celula representa uma parede no labirinto (Compatibilidade)
	TipoCelula_PAREDE,

	//Celula representa uma parede no labirinto
	TipoCelula_PAREDE_SUP,

	//Celula representa uma parede no labirinto
	TipoCelula_PAREDE_LE,

	//Celula representa uma parede no labirinto
	TipoCelula_PAREDE_LD,

	//Celula representa uma parede no labirinto
	TipoCelula_PAREDE_INF,

	//Celula repesenta uma porta
	TipoCelula_PORTA,

	//Celula representa o chao
	TipoCelula_CHAO

} TipoCelula;


typedef struct DLL_IMP_API _celula  
{
	TipoCelula tipo;

	DWORD jogador;
	int objeto;
	int monstro;

} Celula;


