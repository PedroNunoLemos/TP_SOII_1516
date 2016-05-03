#pragma once
#include <windows.h>
#include <tchar.h>

#include "coordenada.h"
#include  "objecto.h"

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




typedef struct DLL_IMP_API _jogador
{

	 int numero;
	 char nome[256];
	 int lentidao;
	 int saude;
	 Coordenada posicao;

	 Item objectos[99];
	 

} Jogador;
