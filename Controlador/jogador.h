#pragma once
#include <windows.h>
#include <tchar.h>

#include "coordenada.h"
#include  "objecto.h"
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


typedef struct DLL_IMP_API _posicao_JogadorMapa
{
	DWORD cor;
	DWORD pidCliente;
	DWORD posicaoOcupada;

}PosicaoJogadorMapa;


typedef struct DLL_IMP_API _jogador
{


	TCHAR nome[256];

	int lentidao;
	int saude;

	Coordenada posicao;
	DWORD pidJogador;

	//int qtdVitaminas;
	//int qtdOranges;
	//int qtdCafeinas;

	int qtdPedras;

	int usarPedra;
	int efeitoCafeina;

} Jogador;
