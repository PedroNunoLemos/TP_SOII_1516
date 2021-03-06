#pragma once
#include <windows.h>
#include <tchar.h>

#include "coordenada.h"


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



typedef enum tipomonstro
{
	DISTRAIDO,
	BULLY

}TipoMonstro;

typedef struct DLL_IMP_API _monstro
{

	int id;
	int energia;
	int lentidao;
	int n_casas;

	TipoMonstro tipo;
	Coordenada posicao;
	TCHAR descricao[10];

	int contadorMovimento;

} Monstro;
