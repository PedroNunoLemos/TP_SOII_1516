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
#include "hist.h"



int AdicionaHist(UtilizadorHist util)
{

	FILE *fp;
	TCHAR Buffer[256];
	TCHAR file[256];
	DWORD dwRet;

	dwRet = GetCurrentDirectory(256, Buffer);

	swprintf(file, 256, TEXT("%s\\hist.dat"), Buffer);

	fp = _tfopen(file, TEXT("wb"));

	if (fp == NULL)
	{
		return 0;
	}


	fwrite(&util, sizeof(UtilizadorHist), 1, fp);


	fclose(fp);

	return 1;

}


Historico *AtualizaHistorico()
{
	FILE *fp;
	Historico *ut;
	UtilizadorHist ht;
	TCHAR Buffer[256];
	TCHAR file[256];
	DWORD dwRet;

	int i = 0;

	ut = malloc(sizeof(Historico));

	dwRet = GetCurrentDirectory(256, Buffer);

	swprintf(file, 256, TEXT("%s\\hist.dat"), Buffer);

	fp = _tfopen(file, TEXT("rb"));

	if (fp == NULL)
	{
		return;
	}




	while (fread(&ht, sizeof(UtilizadorHist), 1, fp) == 1)
	{

		ut->registo[i] = ht;

		if (i < 5)i++;
	}

	for (int j = i; j < 5; j++)
	{
		ut->registo[j].derrota = 0;
		swprintf(ut->registo[j].nome, 50, TEXT(""));

		ut->registo[j].vitoria = 0;
		ut->registo[j].desistencia = 0;
	}

	return  ut;
	
}
