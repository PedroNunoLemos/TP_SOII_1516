#pragma once

#include <Windows.h>


typedef struct utilizador
{
	TCHAR nome[50];
	TCHAR password[20];
	TCHAR util[50];

	int derrotas[10];
	int vitorias[10];


} Utilizador;

int adicionaJogador(Utilizador util);
int autenticaUtilizador(TCHAR *util, TCHAR *pass);
int verificaUtilizador(TCHAR *util);