
#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "uteis.h"

//http://jsfiddle.net/bigbadwaffle/YeazH/

Sala devolveSalaMaisPerto(Labirinto lab, Sala room) {

	int i = 0;
	int  distance = 0;

	Coordenada mid;
	Sala closest;

	mid.x = room.x + (room.w / 2);
	mid.y = room.y + (room.h / 2);

	int closest_distance = 1000;

	for (i = 0; i < 20; i++) {

		Sala check = lab.salas[i];

		if (&check == &room) continue;

		Coordenada check_mid;

		check_mid.x = check.x + (check.w / 2);
		check_mid.y = check.y + (check.h / 2);


		distance = min(abs(mid.x - check_mid.x) - (room.w / 2) - (check.w / 2),
			abs(mid.y - check_mid.y) - (room.h / 2) - (check.h / 2));

		if (distance < closest_distance) {

			closest_distance = distance;
			closest = check;

		}

	}

	return closest;

}

int validaConflitoSala(Labirinto lab, Sala room, int ign) {

	int i = 0;

	for (i = 0; i < 20; i++) {

		if (i == ign) continue;

		Sala check = lab.salas[i];

		if (!((room.x + room.w < check.x) || (room.x > check.x + check.w) || (room.y + room.h < check.y)
			|| (room.y > check.y + check.h)))

			return 1;

	}

	return 0;
}

void validaSalas(Labirinto lab) {

	int i = 0;
	int j = 0;

	for (i = 0; i < 10; i++) {

		for (j = 0; j < lab.tamsalas; j++) {

			Sala room = lab.salas[j];

			while (1) {

				Coordenada old_position;

				old_position.x = room.x;
				old_position.y = room.y;

				if (room.x > 1) room.x--;
				if (room.y > 1) room.y--;

				if ((room.x == 1) && (room.y == 1)) break;

				if (validaConflitoSala(lab, room, j)) {

					room.x = old_position.x;
					room.y = old_position.y;

					break;
				}

			}
		}
	}
}



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

Labirinto CriaSalas(Labirinto lab) {

	int room_count = 20;
	int  min_size = 5;
	int  max_size = 15;
	int i = 0;
	int x = 0;
	int y = 0;

	Labirinto tmp = lab;

	//Cria Salas Aleatorias e mete no labirinto
	for (i = 0; i < room_count; i++) {

		Sala room;

		room.x = aleatorio(1, tmp.tamx - max_size - 1, i);
		room.y = aleatorio(1, tmp.tamx - max_size - 1, i);

		room.w = aleatorio(min_size, max_size, i);
		room.h = aleatorio(min_size, max_size, i);

		if (validaConflitoSala(tmp, room, -1)) {
			i--;
			continue;
		}

		room.w--;
		room.h--;

		tmp.salas[i] = room;

	} //fim criação de salas

	validaSalas(tmp); //compacta salas

	for (i = 0; i < room_count; i++) {

		Sala roomA = tmp.salas[i];

		Sala roomB = devolveSalaMaisPerto(tmp, roomA);

		Coordenada pointA;

		pointA.x = aleatorio(roomA.x, roomA.x + roomA.w, 1);
		pointA.y = aleatorio(roomA.y, roomA.y + roomA.h, 2);

		Coordenada pointB;

		pointB.x = aleatorio(roomB.x, roomB.x + roomB.w, 3);
		pointB.y = aleatorio(roomB.y, roomB.y + roomB.h, 4);

		while ((pointB.x != pointA.x) || (pointB.y != pointA.y)) {

			if (pointB.x != pointA.x) {

				if (pointB.x > pointA.x) pointB.x--;
				else pointB.x++;

			}
			else if (pointB.y != pointA.y) {

				if (pointB.y > pointA.y) pointB.y--;
				else pointB.y++;

			}

			tmp.celula[pointB.x][pointB.y].tipo = TipoCelula_CHAO;

		}
	} // Fim Procesamento das  Salas


	//Por Cada Sala coloca o chao
	for (i = 0; i < room_count; i++) {
		Sala room = tmp.salas[i];
		for (x = room.x; x < room.x + room.w; x++) {
			for (y = room.y; y < room.y + room.h; y++) {
				tmp.celula[x][y].tipo = TipoCelula_CHAO;
			}
		}
	} // Fim preenchimento


	for (x = 0; x < tmp.tamx; x++) {
		for (y = 0; y < tmp.tamy; y++) {
			if (tmp.celula[x][y].tipo == TipoCelula_CHAO) {
				for (int xx = x - 1; xx <= x + 1; xx++) {
					for (int yy = y - 1; yy <= y + 1; yy++) {
						if (tmp.celula[xx][yy].tipo == TipoCelula_VAZIO) tmp.celula[xx][yy].tipo = TipoCelula_PAREDE;
					}
				}
			}
		}
	} // Mete Paredes Nas Salas



	return tmp;
}