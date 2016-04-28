#include <stdio.h>
#include <string.h>

#include "labirinto.h"
#include "consola.h"




int main() {


	//SetWindow(90, 90);

	Labirinto lab;
	lab = CriaLabirinto(lab, 80, 20);

	for (int j = 0; j < 3;j++)
	lab = CriaSala(10+j*20, j+5,lab,j);

	for (int x = 0; x < lab.tamx; x++) {
		for (int y = 0; y < lab.tamy; y++) {

			if (lab.celula[x][y].tipo == TipoCelula_CHAO)
			{
				GoToXY(x, y);
				printf(".");
			}

			else if (lab.celula[x][y].tipo == TipoCelula_VAZIO)
			{
				GoToXY(x, y);
				printf(" ");
			}
			else
			{
				GoToXY(x, y);
				printf("#");
			}
		}

	}

	return 0;
}