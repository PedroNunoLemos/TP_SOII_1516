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



int AdicionaHist(Historico *hist, UtilizadorHist util)
{

	FILE *fp;
	TCHAR Buffer[256];
	TCHAR file[256];
	DWORD dwRet;
	Historico *aux;

	dwRet = GetCurrentDirectory(256, Buffer);

	swprintf(file, 256, TEXT("%s\\hist.dat"), Buffer);

	aux = AtualizaHistorico();

	if (aux == NULL) {

		fp = _tfopen(file, TEXT("wb"));

		if (fp == NULL)
		{
			return 0;
		}

		hist->registo[0] = util;

		for (int i = 1; i < 5; i++)
		{
			hist->registo[i].derrota = 0;
			hist->registo[i].vitoria = 0;
			hist->registo[i].desistencia = 0;
			swprintf(hist->registo[i].nome, 256, TEXT(""));
		}

		fwrite(hist, sizeof(Historico), 1, fp);
		hist->totReg++;

		fclose(fp);


	}
	else
	{


		fp = _tfopen(file, TEXT("wb"));

		if (fp == NULL)
		{
			return 0;
		}

			aux->registo[aux->totReg] = util;
			aux->totReg++;

			fwrite(aux, sizeof(Historico), 1, fp);

		
			hist = aux;

		fclose(fp);
	}




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


	while (fread(ut, sizeof(Historico), 1, fp) == 1);

	fclose(fp);

	return  ut;

}
