
#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "..\Controlador\uteis.h"


//Devolve a Sala mais perto 
Sala devolveSalaMaisPerto(Labirinto lab, Sala sala) {

	int i = 0;
	int  distancia = 0;

	Coordenada meio;
	Sala maisPerto;

	meio.x = sala.x + (sala.w / 2);
	meio.y = sala.y + (sala.h / 2);

	int maisPerto_distancia = 1000;

	for (i = 0; i < 20; i++) {

		Sala salatmp = lab.salas[i];

		if (&salatmp == &sala) continue;

		Coordenada salatmp_meio;

		salatmp_meio.x = salatmp.x + (salatmp.w / 2);
		salatmp_meio.y = salatmp.y + (salatmp.h / 2);


		distancia = min(abs(meio.x - salatmp_meio.x) - (sala.w / 2) - (salatmp.w / 2),
			abs(meio.y - salatmp_meio.y) - (sala.h / 2) - (salatmp.h / 2));

		if (distancia < maisPerto_distancia) {

			maisPerto_distancia = distancia;
			maisPerto = salatmp;

		}

	}

	return maisPerto;

}

//Valida Sala sobreposta
int validaConflitoSala(Labirinto lab, Sala sala, int ign) {

	int i = 0;

	for (i = 0; i < 20; i++) {

		if (i == ign) continue;

		Sala salatmp = lab.salas[i];

		if (!((sala.x + sala.w < salatmp.x) || (sala.x > salatmp.x + salatmp.w) || (sala.y + sala.h < salatmp.y)
			|| (sala.y > salatmp.y + salatmp.h)))

			return 1;

	}

	return 0;
}

//Corrige as salas e ajusta as posicoes
Labirinto corrigeSalas(Labirinto lab) {

	int i = 0;
	int j = 0;
	
	Labirinto tmp=lab;
	
	for (i = 0; i < 10; i++) {

		for (j = 0; j < tmp.tamsalas; j++) {

			Sala sala = tmp.salas[j];

			while (1) {

				Coordenada posicaoAntiga;

				posicaoAntiga.x = sala.x;
				posicaoAntiga.y = sala.y;

				if (sala.x > 1) sala.x--;
				if (sala.y > 1) sala.y--;

				if ((sala.x == 1) && (sala.y == 1)) break;

				if (validaConflitoSala(tmp, sala, j)) {

					sala.x = posicaoAntiga.x;
					sala.y = posicaoAntiga.y;

					break;
				}

			}
			
			tmp.salas[j]=sala;
		}
	}
	
	return tmp;
}


//Metodo que cria as salas no labirinto
Labirinto CriaSalas(Labirinto lab) {

	int numSalas = 20;
	
	int  tamMin = 5;
	int  tamMax = 15;
	
	int i = 0;
	int x = 0;
	int y = 0;

	Labirinto tmp = lab;

	//Cria Salas Aleatorias e mete no labirinto
	for (i = 0; i < numSalas; i++) {

		Sala sala;

		sala.x = aleatorio(1, tmp.tamx - tamMax - 1, i);
		sala.y = aleatorio(1, tmp.tamx - tamMax - 1, i);

		sala.w = aleatorio(tamMin, tamMax, i);
		sala.h = aleatorio(tamMin, tamMax, i);

		if (validaConflitoSala(tmp, sala, -1)) {
			i--;
			continue;
		}

		sala.w--;
		sala.h--;

		tmp.salas[i] = sala;

	} //fim criação de salas

	tmp=corrigeSalas(tmp); //corrige a posicao das salas geradas anteriormente

	//Criamos as portas/Passagens
	for (i = 0; i < numSalas; i++) {

		Sala salaA = tmp.salas[i];

		Sala salaB = devolveSalaMaisPerto(tmp, salaA);

		Coordenada pointA;

		pointA.x = aleatorio(salaA.x, salaA.x + salaA.w, 1);
		pointA.y = aleatorio(salaA.y, salaA.y + salaA.h, 2);

		Coordenada pointB;

		pointB.x = aleatorio(salaB.x, salaB.x + salaB.w, 3);
		pointB.y = aleatorio(salaB.y, salaB.y + salaB.h, 4);

		while ((pointB.x != pointA.x) || (pointB.y != pointA.y)) {

			if (pointB.x != pointA.x) {

				if (pointB.x > pointA.x) pointB.x--;
				else pointB.x++;

			}
			else if (pointB.y != pointA.y) {

				if (pointB.y > pointA.y) pointB.y--;
				else pointB.y++;

			}

			//tmp.celula[pointB.x][pointB.y].tipo = TipoCelula_CHAO;
			tmp.celula[pointB.x][pointB.y].tipo = TipoCelula_PORTA;
		}
	} // Fim Processamento das portas/passagens Salas


	//Por Cada Sala coloca o chao
	for (i = 0; i < numSalas; i++) {
		Sala sala = tmp.salas[i];
		for (x = sala.x; x < sala.x + sala.w; x++) {
			for (y = sala.y; y < sala.y + sala.h; y++) {
				tmp.celula[x][y].tipo = TipoCelula_CHAO;
			}
		}
	} // Fim preenchimento chao das salas


	//Colocamos as Paredes nas Salas
	for (x = 0; x < tmp.tamx; x++) {
		for (y = 0; y < tmp.tamy; y++) {
			if (tmp.celula[x][y].tipo == TipoCelula_CHAO) {
				for (int xx = x - 1; xx <= x + 1; xx++) {
					for (int yy = y - 1; yy <= y + 1; yy++) {
						if (tmp.celula[xx][yy].tipo == TipoCelula_VAZIO)
							tmp.celula[xx][yy].tipo = TipoCelula_PAREDE;
					}
				}
			}
		}
	} // Fim de meter as Paredes Nas Salas



	return tmp;
}

//Metodo principal que cria o labrinto de tamanho tamx,tamy
Labirinto CriaLabirinto(Labirinto lab, int tamx, int tamy) {


	int x = 0;
	int y = 0;
	int j = 0;

	int constroi = 0;

	Labirinto tmp = lab;


	tmp.tamx = tamx;
	tmp.tamy = tamy;
	tmp.tamsalas = 20;

	for (x = 0; x < tamx; x++)
	{
		for (y = 0; y <= tamy; y++)
		{

			tmp.celula[x][y].tipo = TipoCelula_VAZIO;
			tmp.celula[x][y].ponto.x = x;
			tmp.celula[x][y].ponto.y = y;

		}
	}

	for (j = 0; j < tmp.tamsalas; j++)
	{
		tmp.salas[j].x = -1;
		tmp.salas[j].y = -1;
		tmp.salas[j].w = -1;
		tmp.salas[j].h = -1;
	}

	tmp = CriaSalas(tmp);

	return tmp;
}
