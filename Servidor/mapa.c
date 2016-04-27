#include <stdio.h>
#include <string.h>

#include "labirinto.h"





int main() {

	Labirinto lab;
	lab = CriaLabirinto(lab, 80, 20);

	lab = CriaSala(0, 0, 10, 10, lab);

	for (int x = 0; x < lab.tamx; x++) {
		for (int y = 0; y < lab.tamy; y++) {

			if (lab.celula[x][y].tipo == TipoCelula_CHAO)
				printf(".");

			else if (lab.celula[x][y].tipo == TipoCelula_VAZIO)
				printf(" ");

			else printf("#");
		}

	}

	return 0;
}