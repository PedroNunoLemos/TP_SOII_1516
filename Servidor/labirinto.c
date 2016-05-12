
#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "..\Controlador\uteis.h"

int entre_valores(int value, int min, int max)
{
	return (value >= min) && (value <= max);
}


int salaOverlap(Sala *A, Sala* B)
{
	int xOverlap = entre_valores(A->x, B->x, B->x + B->w) ||
		entre_valores(B->x, A->x, A->x + A->w);

	int yOverlap = entre_valores(A->y, B->y, B->y + B->h) ||
		entre_valores(B->y, A->y, A->y + A->h);

	return xOverlap && yOverlap;
}

int salaSobreposta(Labirinto lab, Sala sala) {

	int r = 0;
	int erro = 0;

	for (r = 0; r < lab.tamsalas; r++)
	{
		if (salaOverlap(&(lab.salas[r]), &sala))
			erro = 1;
	}

	return erro;
}

Sala *criaSalaAdjacente(Sala *sala_ant, int lado) {

	int r = 0;
	int ld = 0;

	int x = 0;
	int y = 0;

	int tx = 0;
	int ty = 0;
	int tw = 0;
	int th = 0;

	int erro = 0;

	Sala *sala;
	sala = malloc(sizeof(Sala));

	sala->porta.x = -500;
	sala->porta.y = -500;



	switch (lado)
	{
		//topo
	case 1:


		sala_ant->salaLigada[0] = 1;

		for (r = 0; r < 100; r++) {

			tw = aleatorio(3, 9, r);
			th = aleatorio(3, 9, r);

		}


		ty = sala_ant->y - 1 - th;
		tx = sala_ant->x;

		sala->porta.x = (tx + (tw / 2)) - 1;
		sala->porta.y = ty + th;

		while (!entre_valores(sala->porta.x, tx + 1, tx + tw - 1))
			sala->porta.x--;

		break;

		//esquerda
	case 2:

		sala_ant->salaLigada[1] = 1;

		for (r = 0; r < 100; r++) {
			tw = aleatorio(3, 9, r);
			th = aleatorio(3, 9, r) / 2;
		}


		tx = sala_ant->x - 1 - tw;
		ty = sala_ant->y - 1;

		sala->porta.x = tx + tw;
		sala->porta.y = ty + (th / 2);


		while (!entre_valores(sala->porta.y, ty + 1, ty + th - 1))
			sala->porta.y--;

		break;

		//fundo
	case 3:

		sala_ant->salaLigada[2] = 1;

		for (r = 0; r < 100; r++) {
			tw = aleatorio(3, 9, r);
			th = aleatorio(3, 9, r);
		}


		ty = sala_ant->y + sala_ant->h + 1;
		tx = sala_ant->x;

		sala->porta.x = (tx + (tw / 2));
		sala->porta.y = ty - 1;


		while (!entre_valores(sala->porta.x, tx + 1, tx + tw - 2))
			sala->porta.x--;

		break;

		//direita
	case 4:

		sala_ant->salaLigada[1] = 1;

		for (r = 0; r < 100; r++) {
			tw = aleatorio(3, 9, r);
			th = aleatorio(3, 9, r) / 2;
		}


		tx = sala_ant->x - 1 - tw;
		ty = sala_ant->y - 1;

		sala->porta.x = tx + tw;
		sala->porta.y = ty + (th / 2) - 1;


		while (!entre_valores(sala->porta.y, ty + 1, ty + th - 1))
			sala->porta.y++;

		break;

	default:
		break;
	}

	sala->x = tx;
	sala->y = ty;
	sala->w = tw;
	sala->h = th;
	
	
	sala->id = sala_ant->id + 1;

	if (sala_ant->id == 0)
	{
		sala_ant->porta.x = sala->porta.x;
		sala_ant->porta.y = sala->porta.y;
	}

	return sala;

}


Labirinto *CriaLabirinto(int tamx, int tamy, int salas) {

	int x = 0;
	int y = 0;
	int r = 0;
	int i = 0;
	int cnt = 0;

	Labirinto *lab;
	lab = malloc(sizeof(Labirinto));

	lab->tamsalas = salas;
	lab->tamx = tamx;
	lab->tamy = tamy;


	for (x = 0; x < tamx; x++)
	{
		for (y = 0; y <= tamy; y++)
		{

			lab->celula[x][y].tipo = TipoCelula_VAZIO;
			lab->celula[x][y].ponto.x = x;
			lab->celula[x][y].ponto.y = y;

		}
	}

	Sala center;

	center.w = aleatorio(4, 8, 2);
	center.h = aleatorio(4, 8, 2);

	center.x = (tamx / 2) - (center.w / 2);
	center.y = (tamy / 2) - (center.h / 2);
	center.id = 0;

	lab->salas[0] = center;


	for (r = 1; r < lab->tamsalas; r++)
	{

		Sala *tmp = NULL;

		do {

			free(tmp);

			i = aleatorio(1, 4, cnt);

			tmp = criaSalaAdjacente(&(lab->salas[r - 1]), i);

			cnt++;

		} while (!tmp || salaSobreposta(*lab, *tmp) ||
			tmp->x <= 5 || tmp->y <= 5
			|| tmp->x + tmp->w >= lab->tamx - 5
			|| tmp->y + tmp->h >= lab->tamy - 5);

		if (tmp)

			lab->salas[r] = *tmp;
	}



	//Por Cada Sala coloca o chao
	for (i = 0; i < salas; i++) {

		Sala *sala = &(lab->salas[i]);

		for (x = sala->x; x < sala->x + sala->w; x++) {
			for (y = sala->y; y < sala->y + sala->h; y++) {
				lab->celula[x][y].tipo = TipoCelula_CHAO;
			}
		}
	} // Fim preenchimento chao das salas

	  //Colocamos as Paredes nas Salas
	for (x = 0; x < lab->tamx; x++) {
		for (y = 0; y < lab->tamy; y++) {
			if (lab->celula[x][y].tipo == TipoCelula_CHAO) {
				for (int xx = x - 1; xx <= x + 1; xx++) {
					for (int yy = y - 1; yy <= y + 1; yy++) {
						if (lab->celula[xx][yy].tipo == TipoCelula_VAZIO)
							lab->celula[xx][yy].tipo = TipoCelula_PAREDE;
					}
				}
			}
		}
	} // Fim de meter as Paredes Nas Salas


	  //Por Cada Sala coloca portas
	for (i = 0; i < salas; i++) {
		for (x = 0; x < lab->tamx; x++) {
			for (y = 0; y < lab->tamy; y++) {

				if (lab->salas[i].porta.x == x  && lab->salas[i].porta.y == y)
					lab->celula[x][y].tipo = TipoCelula_PORTA;

			}
		}

	}


	return lab;

}
