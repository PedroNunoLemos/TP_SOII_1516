#pragma once

#include <Windows.h>


typedef struct utilizadorhist
{
	TCHAR nome[200];


	int derrota;
	int vitoria;
	int desistencia;

} UtilizadorHist;


typedef struct historico
{
	UtilizadorHist registo[5];

} Historico;

int AdicionaHist(UtilizadorHist util);
void AtualizaHistorico(Historico *hist);