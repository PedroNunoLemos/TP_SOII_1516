#include "tui.h"
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>



void caixa(int x, int y, int x2, int y2, int back, int fore)
{
	int i = 0;
	int j = 0;

	for (i = x; i < x2; i++)
	{
		GoToXY(i, y);
		_tprintf("*");
	}



	for (i = y; i < y2; i++)
	{
		GoToXY(x2, i);
		_tprintf("*");
	}



	for (i = x2; i > x; i--)
	{
		GoToXY(i, y2);
		_tprintf("*");
	}


	for (i = y2; i > y; i--)
	{
		GoToXY(x, i);
		_tprintf("*");
	}

	for (i = x + 1; i < x2 - 1; i++)
	{
		for (j = y + 1; j < y2 - 1; j++)
		{
			GoToXY(i, j);
			_tprintf(" ");
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
			_tprintf(" ");
		}
	}



}

//void imprimeLabirinto(int x, int y, int mx, int my,  Jogo lab) {
//
//
//
//
//
//}
