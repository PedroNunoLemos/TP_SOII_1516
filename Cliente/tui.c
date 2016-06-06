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

	for (ix = 0; ix < MAXVISX; ix++)
	{
		for (iy = 0; iy < MAXVISY; iy++) {


			GoToXY(x + ix, y + iy);

			//if (lab->jogadoresMapa[ix][iy].posicaoOcupada != 0) {

			//	if (lab->jogadoresMapa[ix][iy].posicaoOcupada != lab->jogador.pidJogador) {
			//		setForeGroundAndBackGroundColor(Color_White, 6);
			//		_tprintf(TEXT("0"));
			//	}
			//}

		//for (k = 0; k < 5; k++) {

		//	if (lab->objectosMapa[ix][iy].objecto[k].tipo == Tipo_Vitamina)
		//	{
		//		setForeGroundAndBackGroundColor(Color_LightRed, 6);  _tprintf(TEXT("V"));
		//	}

		//	if (lab->objectosMapa[ix][iy].objecto[k].tipo == Tipo_Cafeina)
		//	{
		//		setForeGroundAndBackGroundColor(Color_LightYellow, 6);  _tprintf(TEXT("C"));
		//	}

		//	if (lab->objectosMapa[ix][iy].objecto[k].tipo == Tipo_OrangeBull)
		//	{
		//		setForeGroundAndBackGroundColor(Color_Green, 6);  _tprintf(TEXT("O"));
		//	}

		//	if (lab->objectosMapa[ix][iy].objecto[k].tipo == Tipo_Pedra)
		//	{
		//		setForeGroundAndBackGroundColor(Color_LightBlue, 6);  _tprintf(TEXT("P"));
		//	}


		//}

			switch (lab->mapa[ix][iy].tipo)
			{

			case TipoCelula_PORTA:
			case TipoCelula_CHAO:

				setForeGroundAndBackGroundColor(0, 6); _tprintf(TEXT(" "));

				break;
			case TipoCelula_PAREDE:
				setForeGroundAndBackGroundColor(7, 3);
				_tprintf(TEXT(" "));
				break;
			default:
				setForeGroundAndBackGroundColor(Color_Black, Color_Black); _tprintf(TEXT(" "));
				break;
			}



		}
	}


	GoToXY(x + (MAXVISX/2), y + (MAXVISY/2));
	setForeGroundAndBackGroundColor(Color_Blue, 6);
	_tprintf(TEXT("@"));


}
