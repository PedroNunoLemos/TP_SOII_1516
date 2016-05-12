#include "tui.h"


int validaMovimento(Labirinto lab, int x, int y) {


	

	if ((
		lab.celula[x][y].tipo == TipoCelula_CHAO ||
		lab.celula[x][y].tipo == TipoCelula_PORTA))
		return 1; else return 0;


}

void imprimeLabirinto(int x, int y, int mx, int my, int visx, int visy, Labirinto tmp) {


	int ix = 0;
	int iy = 0;
	int px;
	int py;

	int cx = 0;
	int cy = 0;

	Labirinto lab = tmp;


	px = x + (visx / 2);
	py = y + (visy / 2);

	for (ix = 0; ix < tmp.tamx; ix++)
	{
		for (iy = 0; iy < tmp.tamy; iy++) {

			if ((ix >= mx - (visx / 2) && iy >= my - (visy / 2))
				&& (ix <= mx + (visx / 2) && iy <= my + (visy / 2))
				)
			{

				GoToXY(x + (ix - mx + (visx / 2)), y + (iy - my + (visy / 2)));


				if (lab.celula[ix][iy].tipo == TipoCelula_CHAO
					|| lab.celula[ix][iy].tipo == TipoCelula_PORTA)
				{
					setForeGroundAndBackGroundColor(0, 6); printf(".");
				}

				else if (lab.celula[ix][iy].tipo == TipoCelula_PAREDE) { setcolor(Color_White); printf("*"); }
				else if (lab.celula[ix][iy].tipo == TipoCelula_VAZIO) { setcolor(Color_Gray); printf(","); }
				else { setcolor(Color_Black); printf(" "); }


			}
		}

	}


	GoToXY(px, py);
	setcolor(Color_White);

	printf("@");

	GoToXY(0, 0);
	printf("%d %d %d ", ix, mx + visx, my + visy);

}
