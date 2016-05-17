#include "tui.h"
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#include "../Controlador/constantes.h"

void caixa(int x, int y, int x2, int y2, int back, int fore)
{
	int i = 0;
	int j = 0;

	for (i = x; i < x2; i++)
	{
		GoToXY(i, y);
		_tprintf(TEXT("*"));
	}



	for (i = y; i < y2; i++)
	{
		GoToXY(x2, i);
		_tprintf(TEXT("*"));
	}



	for (i = x2; i > x; i--)
	{
		GoToXY(i, y2);
		_tprintf(TEXT("*"));
	}


	for (i = y2; i > y; i--)
	{
		GoToXY(x, i);
		_tprintf(TEXT("*"));
	}

	for (i = x + 1; i < x2 - 1; i++)
	{
		for (j = y + 1; j < y2 - 1; j++)
		{
			GoToXY(i, j);
			_tprintf(TEXT(" "));
		}
	}


}

void limpaArea(int x, int y, int x2, int y2) {

	int i = 0;
	int j = 0;


	for (i = x; i < x2 + 1; i++)
	{
		for (j = y; j < y2 + 1; j++)
		{
			GoToXY(i, j);
			_tprintf(TEXT(" "));
		}
	}



}

void imprimeLabirinto(int x, int y, JogoCliente lab) {

	int ix = 0;
	int iy = 0;

	int k = 0;

	for (ix = 0; ix < 15; ix++)
	{
		for (iy = 0; iy < 15; iy++) {


			GoToXY(x + ix, y + iy);


			if (lab.mapa[ix][iy].tipo == TipoCelula_CHAO
				|| lab.mapa[ix][iy].tipo == TipoCelula_PORTA)
			{
				setForeGroundAndBackGroundColor(0, 6); _tprintf(TEXT(" "));
			}

			else if (lab.mapa[ix][iy].tipo == TipoCelula_PAREDE) {
				setForeGroundAndBackGroundColor(7, 3);
				_tprintf(TEXT(" "));
			}
			else if (lab.mapa[ix][iy].tipo == TipoCelula_VAZIO) { setcolor(Color_Gray); _tprintf(TEXT(" ")); }
			else { setcolor(Color_Black); _tprintf(TEXT(" ")); }



		}
	}


	GoToXY(x + 7, y + 7);
	setForeGroundAndBackGroundColor(Color_Blue, 6);
	_tprintf(TEXT("@"));




}
