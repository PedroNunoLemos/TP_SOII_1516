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


int adicionaJogador(Utilizador util)
{

	FILE *fp;

	fp = fopen("bd.dat", "wb");

	if (fp == NULL)
	{
		return 0;
	}


	fwrite(&util, sizeof(Utilizador), 1, fp);


	fclose(fp);


}


int autenticaUtilizador(TCHAR *util, TCHAR *pass)
{
	FILE *fp;
	Utilizador ut;

	fp = fopen("bd.dat", "rb");

	if (fp == NULL)
	{
		return -1;
	}


	while (fread(&ut, sizeof(Utilizador), 1, fp) == 1)
	{
		
		if (_tcscmp(util, ut.nome) == 0 && _tcscmp(pass, ut.password) == 0) {
		



		}

	}


}

int verificaUtilizador(TCHAR *util)
{
	TCHAR key[TAM] = REGISTRY_KEY;
	HKEY hKey;

	wcscat_s(key, _tcslen(util)*sizeof(TCHAR), util);

	if (RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		return 0;

	return 1;
}