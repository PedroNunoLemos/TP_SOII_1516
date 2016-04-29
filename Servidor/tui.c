#include "tui.h"

void imprimeLabirinto(int x, int y, int mx, int my, int visx,int visy, Labirinto lab) {


	int ix = 0;
	int iy = 0;

	GoToXY(x, y);

	if (mx <= 0)mx = 0;
	if (my <= 0)my = 0;

	if (mx >= lab.tamx)mx = lab.tamx;
	if (my >= lab.tamy)my = lab.tamy;

	for (ix = 0; ix < visx; ix++)
	{
		for (iy = 0; iy < visy; iy++) {

			GoToXY(x + ix, y + iy);

			if (lab.celula[mx + ix][my + iy].tipo == TipoCelula_CHAO) printf(".");
			else if (lab.celula[mx + ix][my + iy].tipo == TipoCelula_PAREDE_HZ) printf("-");
			else if (lab.celula[mx + ix][my + iy].tipo == TipoCelula_PAREDE_VT) printf("|");
			else if (lab.celula[mx + ix][my + iy].tipo == TipoCelula_VAZIO) printf(" ");
			else printf(" ");

		}

	}




}