#pragma once

#include <windows.h>
#include <tchar.h>

#include "coordenada.h"
#include "jogador.h"
#include "celula.h"
#include "constantes.h"


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



typedef struct DLL_IMP_API _jogoCliente
{

	int id;

	int comando;

	int respostaComando;
	int moveuDirecao;


	Celula mapa[15][15];

	DWORD pidCliente;
	Jogador jogador;

	TCHAR mensagem[256];


} JogoCliente;


