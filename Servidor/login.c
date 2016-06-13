#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\memoria.h"

#include "JogoServidor.h"
#include "servidor.h"
#include "motorjogo.h"
#include "login.h"



int AdicionaHist(UtilizadorHist util)
{

	FILE *fp;

	fopen_s(&fp, "hist.dat", "wb");

	if (fp == NULL)
	{
		return 0;
	}


	fwrite(&util, sizeof(UtilizadorHist), 1, fp);


	fclose(fp);

	return 1;

}


void AtualizaHistorico(Historico *hist)
{
	FILE *fp;
	Historico ut;
	UtilizadorHist ht;
	int i = 0;

	fopen_s(&fp,"hist.dat", "rb");

	if (fp == NULL)
	{
		return;
	}


	while (fread(&ht, sizeof(UtilizadorHist), 1, fp) == 1)
	{
		ut.registo[i] = ht;

		if (i < 5)i++;
	}

	for (int j = i; j < 5; j++)
	{
		ut.registo[i].derrota = 0;
		swprintf(ut.registo[i].nome, 50, TEXT(""));

		ut.registo[i].vitoria = 0;
		ut.registo[i].desistencia = 0;
	}

	hist = &ut;

}
