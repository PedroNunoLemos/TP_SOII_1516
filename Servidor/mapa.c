#include <stdio.h>
#include <string.h>

#include "labirinto.h"
#include "consola.h"




int main() {

	int j = 0;

	//SetWindow(90, 90);

	Labirinto lab;
	lab = CriaLabirinto(lab, 80, 80);



		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {

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
				else if (lab.celula[x][y].tipo == TipoCelula_PORTA)
				{
					GoToXY( x,  y);
					printf("D");
				}

				else if (lab.celula[x][y].tipo == TipoCelula_PAREDE_HZ)
				{
					GoToXY(x, y);
					printf("-");
				}

				else if (lab.celula[x][y].tipo == TipoCelula_PAREDE_VT)
				{
					GoToXY(x, y);
					printf("|");
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