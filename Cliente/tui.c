#include "tui.h"

//void imprimeLabirinto(int x, int y, int mx, int my,  Jogo lab) {
//
//
//	int ix = 0;
//	int iy = 0;
//
//	GoToXY(x, y);
//
//	if (mx <= 0)mx = 0;
//	if (my <= 0)my = 0;
//
//	if (mx >= lab.tam)mx = lab.tam;
//	if (my >= lab.tam)my = lab.tam;
//
//	for (ix = 0; ix < lab.tam; ix++)
//	{
//		for (iy = 0; iy < lab.tam; iy++) {
//
//			GoToXY(x + ix, y + iy);
//
//			if (lab.mapaAtual[mx + ix][my + iy].tipo == TipoCelula_CHAO) printf(".");
//			else if (lab.mapaAtual[mx + ix][my + iy].tipo == TipoCelula_PAREDE) printf("#");
//			else if (lab.mapaAtual[mx + ix][my + iy].tipo == TipoCelula_VAZIO) printf(" ");
//			else printf(" ");
//
//		}
//
//	}
//
//
//
//
//}
