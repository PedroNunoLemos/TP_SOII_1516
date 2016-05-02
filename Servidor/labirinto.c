
#include <stdio.h>
#include <stdlib.h>

#include "labirinto.h"
#include "uteis.h"

//http://jsfiddle.net/bigbadwaffle/YeazH/

Labirinto CriaLabirinto(Labirinto lab, int tamx, int tamy) {


	int x = 0;
	int y = 0;
	int j = 0;
	
	int constroi = 0;

	Labirinto tmp = lab;


	tmp.tamx = tamx;
	tmp.tamy = tamy;


	for (x = 0; x < tamx; x++)
	{
		for (y = 0; y <= tamy; y++)
		{
			
			tmp.celula[x][y].tipo = TipoCelula_VAZIO;
			tmp.celula[x][y].ponto.x = x;
			tmp.celula[x][y].ponto.y = y;
			
		}
	}
	
	for (j = 0; j < 20 ; j++)
	{
		tmp.salas[j].x=-1;
		tmp.salas[j].y=-1;
		tmp.salas[j].w=-1;
		tmp.salas[j].h=-1;
	}

	return tmp;
}

Labirinto CriaSalas(Labirinto lab){
	
	int room_count = 20;
        int  min_size = 5;
        int  max_size = 15;

        Labirinto tmp = lab;
        
 for (var i = 0; i < room_count; i++) {
 	
           Sala room;

            room.x = aleatorio(1, this.map_size - max_size - 1,i);
            room.y = aleatorio(1, this.map_size - max_size - 1,i);
            room.w = aleatorio(min_size, max_size,i);
            room.h = aleatorio(min_size, max_size,i);

            if (DoesCollide(room)) {
                i--;
                continue;
            }
            
            room.w--;
            room.h--;

            tmp.salas[i]=room;
            
        }
	
	return tmp;
}

Labirinto  CriaSala(int inix, int iniy, Labirinto lab, int seed) {

	int x = 0;
	int y = 0;
	int tamx = 0;
	int tamy = 0;

	int prt = 0;

	Labirinto tmp = lab;



	tamx = aleatorio(5, 9, seed);
	tamy = aleatorio(4, 9, seed);

	for (x = 0; x < tamx; x++)
	{
		for (y = 0; y <= tamy; y++)
		{



			//define celula tipo
			Celula novacel;
			novacel.tipo = TipoCelula_VAZIO;

			if (x == 0 || x == tamx - 1)
			{
				if (y == 0 && x == 0) novacel.tipo = TipoCelula_PAREDE_SE;
				else if (y == tamy && x == 0) novacel.tipo = TipoCelula_PAREDE_IE;
				else if (y == 0 && x == tamx - 1) novacel.tipo = TipoCelula_PAREDE_SD;
				else if (y == tamy && x == tamx - 1)novacel.tipo = TipoCelula_PAREDE_ID;
				else {

					novacel.tipo = TipoCelula_PAREDE_VT;

				}
			}

			else
			{
				if (y == 0) {
					novacel.tipo = TipoCelula_PAREDE_HZ;
				}
				else if (y == tamy)
				{
					novacel.tipo = TipoCelula_PAREDE_HZ;
				}
				else
					novacel.tipo = TipoCelula_CHAO;
			}

			// fim definição tipo de celula


			//mete no labirinto a nova celula 

			tmp.celula[inix + x][iniy + y] = novacel;


		} // fim for y
	} //fim for x

	return tmp;
}

