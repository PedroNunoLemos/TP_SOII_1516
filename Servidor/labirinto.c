
#include <stdio.h>
#include <stdlib.h>

#include "labirinto.h"
#include "uteis.h"

Labirinto CriaLabirinto(Labirinto lab, int tamx, int tamy) {


	int x = 0;
	int y = 0;

	Labirinto tmp = lab;

	tmp.tamx = tamx;
	tmp.tamy = tamy;



	for (x = 0; x < tamx; x++)
	{
		for (y = 0; y <= tamy; y++)
		{

			tmp.celula[x][y].tipo = TipoCelula_VAZIO;
			tmp.celula[x][y].ponto.x = x;
			tmp.celula[x][y].ponto.y = y;
		}
	}

	return tmp;
}

Labirinto  CriaSala(int inix, int iniy, Labirinto lab,int seed) {

	int x = 0;
	int y = 0;
	int tamx = 0;
	int tamy = 0;

	Labirinto tmp = lab;



	tamx = aleatorio(10, 16, seed);
	tamy = aleatorio(4, 6, seed);


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
				else if (y == 0 && x == tamx - 1) novacel.tipo = TipoCelula_PAREDE_SD;
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

			// fim defini��o tipo de celula

			//mete no labirinto a nova celula 

			tmp.celula[inix + x][iniy + y] = novacel;


		} // fim for y
	} //fim for x

	return tmp;
}

