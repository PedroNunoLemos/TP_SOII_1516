#include "tui.h"

void imprimeLabirinto(int x, int y, int mx, int my, int vis, Labirinto lab) {


	int ofx = 0;
	int ofy = 0;
	int ix = 0;
	int iy = 0;



	GoToXY(x, y);




	for (ix = 0; ix < vis; ix++)
	{
		for (iy = 0; iy < vis; iy++) {

			GoToXY(x + ix, y + iy);

			if (lab.celula[mx + ix][my + iy].tipo == TipoCelula_CHAO) printf(".");
			else if (lab.celula[mx + ix][my + iy].tipo == TipoCelula_PAREDE_HZ) printf("-");
			else if (lab.celula[mx + ix][my + iy].tipo == TipoCelula_PAREDE_VT) printf("|");
			else if (lab.celula[mx + ix][my + iy].tipo == TipoCelula_VAZIO) printf(" ");
			else printf("#");

		}

		ofx++;
		ofy++;
	}




}