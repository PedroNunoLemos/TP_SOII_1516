
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

Labirinto* CriaLabirinto(Labirinto *lab, int tamx, int tamy, int salas) {


	int tamsalaw = 10;
	int tamsalah = 10;
	int i = 0;
	int r = 0;
	int numSala = 0;
	int ok = 0;
	int  corr_ajustar = 10;
	int cnt = 0;
	int x = 0;
	int y = 0;

	int xx = 0;
	int yy = 0;



	Coordenada *salasLigadas;
	salasLigadas = malloc(sizeof(Coordenada) * tamx * tamy);


	lab = malloc(sizeof(Labirinto));
	lab->tamx = tamx;
	lab->tamy = tamy;

	lab->tamsalas = salas;

	//inicializa labirinto
	for (y = 0; y < tamy; y++) {
		for (x = 0; x < tamx; x++) {

			lab->celula[y][x].tipo = TipoCelula_VAZIO;
			lab->celula[y][x].ponto.x = x;
			lab->celula[y][x].ponto.y = y;

		}
	}

	for (xx = 0; xx < salas; xx++) {

		Sala* sala;
		sala = malloc(sizeof(Sala));

		sala->x = -1;
		sala->y = -1;
		sala->w = -1;
		sala->h = -1;

		lab->salas[xx] = *sala;

	}


	// cria salas no labirinto
	for (i = 0; i < salas; i++) {

		Sala *sala = malloc(sizeof(Sala));

		ok = 0;

		do
		{

			sala->x = aleatorio(1, tamx - 1, i);
			sala->y = aleatorio(1, tamx - 1, i);

			sala->w = aleatorio(3, 5, 3);
			sala->h = aleatorio(3, 5, 7);


			// valida confins
			if (sala->x + sala->w >= tamx || sala->y + sala->h >= tamy)ok = 1;

			for (r = 0; r < salas; r++) {

				if ((salaOverlap(sala, &lab->salas[r])))
					ok = 1;
			}


		} while (sala->x < 0 && sala->y < 0 && !ok);


		// valida outras salas no labirinto

		lab->salas[i] = *sala;

		//fim criação e colocação de sala


	} // fim criação de salas	

	//liga  as salas com corredores e coloca no labirinto


	for (i = 0; i < salas; i++)
	{
		Sala *salaA = &(lab->salas[i]);

		numSala = i;

		int tn = 0;
		while (numSala == i) {
			numSala = aleatorio(1, lab->tamsalas, tn);
			tn++;
		}

		Sala *salaB = &(lab->salas[numSala]);

		//Aumentar o numero aumenta o corredor
		//Diminuir o numero encolhe o corredor

		corr_ajustar = 10;

		Coordenada *pontoA;

		pontoA = malloc(sizeof(Coordenada));


		pontoA->x = aleatorio(salaA->x, salaA->x + salaA->w, 1);
		pontoA->y = aleatorio(salaA->y, salaA->y + salaA->h, 2);

		Coordenada *pontoB;

		pontoB = malloc(sizeof(Coordenada));

		pontoB->x = aleatorio(salaB->x, salaB->x + salaB->w, 3);
		pontoB->y = aleatorio(salaB->y, salaB->y + salaB->h, 4);

		//Algoritimo drunken/lazy walk   

		cnt = 0;

		while (pontoB->x != pontoA->x || pontoB->y != pontoA->y) {

			int num = aleatorio(1, 100, cnt);

			if (num < corr_ajustar) {

				if (pontoB->x != pontoA->x) {

					if (pontoB->x > pontoA->x) {
						pontoB->x--;
					}
					else {
						pontoB->x++;
					}

				}

			}
			else if (pontoB->y != pontoA->y) {

				if (pontoB->y > pontoA->y) {
					pontoB->y--;
				}
				else {
					pontoB->y++;
				}

			}


			//preenche ponteiro de salas ligadas
			if (pontoB->x < tamx && pontoB->y < tamy) {

				cnt++;

				//salasLigadas[cnt].x = pontoA->x;
				//salasLigadas[cnt].y = pontoB->y;

			}// fim ponteiro salas ligadas

		}// fim lazy walk


		free(pontoA);
		free(pontoB);
	} // Fim ligação das Salas (i)


	// preenche as salas no labirinto
	for (i = 0; i < salas; i++) {
		for (y = lab->salas[i].y; y < lab->salas[i].y + lab->salas[i].h; y++) {
			for (x = lab->salas[i].x; x < lab->salas[i].x + lab->salas[i].w; x++) {
				lab->celula[y][x].tipo = TipoCelula_CHAO;//Chao
			}
		}
	} // fim preechimento salas labirinto



	  // coloca os corredores no labririnto
/*	for (i = 0; i < cnt; i++)
	{
		x = salasLigadas[i].x;
		y = salasLigadas[i].y;

		if (y > 1 && x > 1)
			lab->celula[y][x].tipo = TipoCelula_CHAO;//corredor
	}

	*/

	//Coloca as paredes
	for (y = 0; y < tamy; y++) {
		for (x = 0; x < tamx; x++) {

			if (lab->celula[y][x].tipo == TipoCelula_VAZIO) { //compara com vazio

				int parede = 0;

				for (yy = y - 2; yy < y + 2; yy++) {
					for (xx = x - 2; xx < x + 2; xx++) {
						if (xx > 0 && yy > 0 && xx < tamx && yy < tamy) {
							if (lab->celula[yy][xx].tipo == TipoCelula_CHAO) {
								//|| lab->celula[yy][xx] == 2

								lab->celula[y][x].tipo = TipoCelula_PAREDE;// trocar por parede
								parede = 1;
							}
						}
					}

					if (parede) { break; }

				}//fim yy

			} // fim percorrer labirinto
		} // fim x

	} //fim  y paredes

	return lab;
}
