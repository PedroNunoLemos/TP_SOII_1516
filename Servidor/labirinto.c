
#include <stdio.h>
#include <stdlib.h>

#include "labirinto.h"


Labirinto  CriaSala(int inix, int iniy, int tamx, int tamy, Labirinto lab) {

	int x = 0;
	int y = 0;
	int j = 0;

	Labirinto tmp=lab;
	
	for (x = 0; x < tamx; x++)
	{
		for (y = 0; y <= tamy; y++)
		{


	
			//define celula tipo
			Celula novacel;
			novacel.tipo = TipoCelula_VAZIO;

			if (x == 0 || x == tamx - 1)
			{
				if (y == 0 && x == 0) novacel.tipo = TipoCelula_PAREDE_SE;
				else if (y == tamy && x == 0) novacel.tipo = TipoCelula_PAREDE_IE;
				else if (y == 0 && x == tamx - 1) novacel.tipo = TipoCelula_PAREDE_SD ;
				else if (y == tamy && x == tamx - 1)novacel.tipo = TipoCelula_PAREDE_ID;
				else novacel.tipo = TipoCelula_PAREDE_HZ;
			}

			else
			{
				if (y == 0)
					novacel.tipo = TipoCelula_PAREDE_VT;
				else if (y == tamy)
					novacel.tipo = TipoCelula_PAREDE_VT;
				else
					novacel.tipo = TipoCelula_CHAO;
			}

			// fim definição tipo de celula
			tmp.celulas[x][y] = novacel;
		
		
		} // fim for y
	} //fim for x

	return tmp;
}

