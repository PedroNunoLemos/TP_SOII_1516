#include "tui.h"
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#include "../Controlador/constantes.h"
#include "../Controlador/jogo.h"

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

void imprimeLabirinto(int x, int y, JogoCliente *lab) {

	int ix = 0;
	int iy = 0;

	int k = 0;
	char c = ' ';

	for (ix = 0; ix < MAXVISX; ix++)
	{
		for (iy = 0; iy < MAXVISY; iy++) {


			GoToXY(x + ix, y + iy);




			switch (lab->mapa[ix][iy].tipo)
			{

			case TipoCelula_PORTA:
			case TipoCelula_CHAO:

				setForeGroundAndBackGroundColor(0, 6);
				c = ' ';

				switch (lab->mapa[ix][iy].objeto)
				{
				case Tipo_Pedra:
					setForeGroundAndBackGroundColor(Color_Yellow, 6);
					c = 'p';
					break;

				case Tipo_Vitamina:
					setForeGroundAndBackGroundColor(Color_LightRed, 6);
					c = 'v';
					break;

				case Tipo_Cafeina:
					setForeGroundAndBackGroundColor(Color_LightMagenta, 6);
					c = 'c';
					break;

				default:
					break;

				}


				_tprintf(TEXT("%c"), c);

				break;
			case TipoCelula_PAREDE:
				setForeGroundAndBackGroundColor(7, 3);
				_tprintf(TEXT(" "));
				break;
			default:
				setForeGroundAndBackGroundColor(Color_Black, Color_Black);
				_tprintf(TEXT("."));
				break;
			}


		}
	}


	GoToXY(x + (MAXVISX / 2), y + (MAXVISY / 2));
	setForeGroundAndBackGroundColor(Color_Blue, 6);
	_tprintf(TEXT("@"));


}
