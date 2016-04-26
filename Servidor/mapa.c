#include <stdio.h>
#include <string.h>

#include "labirinto.h"





int main() {

	Labirinto lab;
	lab = CriaSala(0, 0, 10, 10, lab);

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (lab.celulas[x][y].tipo == TipoCelula_CHAO)
				printf(".");
			else if (lab.celulas[x][y].tipo == TipoCelula_VAZIO)
				printf(" ");
			else printf("#");
		}

	}

	return 0;
}