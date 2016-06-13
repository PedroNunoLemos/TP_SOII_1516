#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>


#include "..\Controlador\memoria.h"
#include "..\Controlador\jogo.h"
#include "..\Controlador\uteis.h"
#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"


#include "motorMonstro.h"



Coordenada monstroPosicaoAdjLivre(JogoServidor *jogo, int mid) {

	int i = 0;
	int x = 0;
	int y = 0;

	Coordenada res;

	Coordenada mp = jogo->monstros[mid].posicao;

	res.x = -1;
	res.x = -1;

	for (x = 0; x < jogo->mapa.tamx; x++)
	{
		for (y = 0; y < jogo->mapa.tamy; y++)
		{

			Celula tc = jogo->mapa.celula[x][y];

			if (mp.x == x && mp.y == y) {

				if (jogo->mapa.celula[x - 1][y - 1].monstro == -1 &&
					jogo->mapa.celula[x - 1][y - 1].jogador == 0 &&
					(jogo->mapa.celula[x - 1][y - 1].tipo == TipoCelula_CHAO ||
						jogo->mapa.celula[x - 1][y - 1].tipo == TipoCelula_PORTA
						)
					)
				{
					res.x = x - 1;
					res.y = y - 1;

					return res;

				}


				if (jogo->mapa.celula[x - 1][y].monstro == -1 &&
					jogo->mapa.celula[x - 1][y].jogador == 0 &&
					(jogo->mapa.celula[x - 1][y].tipo == TipoCelula_CHAO ||
						jogo->mapa.celula[x - 1][y].tipo == TipoCelula_PORTA
						))
				{
					res.x = x - 1;
					res.y = y;

					return res;

				}

				if (jogo->mapa.celula[x - 1][y + 1].monstro == -1 &&
					jogo->mapa.celula[x - 1][y + 1].jogador == 0 &&
					(jogo->mapa.celula[x - 1][y + 1].tipo == TipoCelula_CHAO ||
						jogo->mapa.celula[x - 1][y + 1].tipo == TipoCelula_PORTA
						)
					)
				{
					res.x = x - 1;
					res.y = y + 1;

					return res;

				}

				if (jogo->mapa.celula[x][y - 1].monstro == -1 &&
					jogo->mapa.celula[x][y - 1].jogador == 0 &&
					(jogo->mapa.celula[x][y - 1].tipo == TipoCelula_CHAO ||
						jogo->mapa.celula[x][y - 1].tipo == TipoCelula_PORTA
						)
					)
				{
					res.x = x;
					res.y = y - 1;

					return res;

				}


				if (jogo->mapa.celula[x + 1][y - 1].monstro == -1 &&
					jogo->mapa.celula[x + 1][y - 1].jogador == 0 &&
					(jogo->mapa.celula[x + 1][y - 1].tipo == TipoCelula_CHAO ||
						jogo->mapa.celula[x + 1][y - 1].tipo == TipoCelula_PORTA
						)
					)
				{
					res.x = x + 1;
					res.y = y - 1;

					return res;

				}




				if (jogo->mapa.celula[x + 1][y].monstro == -1 &&
					jogo->mapa.celula[x + 1][y].jogador == 0 &&
					(jogo->mapa.celula[x + 1][y].tipo == TipoCelula_CHAO ||
						jogo->mapa.celula[x + 1][y].tipo == TipoCelula_PORTA
						)
					)
				{
					res.x = x + 1;
					res.y = y;

					return res;

				}


				if (jogo->mapa.celula[x + 1][y + 1].monstro == -1 &&
					jogo->mapa.celula[x + 1][y + 1].jogador == 0 &&
					(jogo->mapa.celula[x + 1][y + 1].tipo == TipoCelula_CHAO ||
						jogo->mapa.celula[x + 1][y + 1].tipo == TipoCelula_PORTA
						)
					)
				{
					res.x = x + 1;
					res.y = y + 1;

					return res;

				}

				if (jogo->mapa.celula[x][y + 1].monstro == -1 &&
					jogo->mapa.celula[x][y + 1].jogador == 0 &&
					(jogo->mapa.celula[x][y + 1].tipo == TipoCelula_CHAO ||
						jogo->mapa.celula[x][y + 1].tipo == TipoCelula_PORTA
						)
					)
				{
					res.x = x;
					res.y = y + 1;

					return res;

				}


			}
		}
	}

	res = mp;

	return res;

}

int existeJogadorNaPosicao(JogoServidor *jogo, int  x, int y) {


	int i = 0;

	for (i = 0; i < jogo->jogadoresLigados; i++) {
		if (jogo->clientes[i].jogo.id >= 0)
		{

			if (jogo->clientes[i].jogo.jogador.posicao.x == x && jogo->clientes[i].jogo.jogador.posicao.y == y)
				return 1;
		}
	}

	return 0;

}

Coordenada PosicaoIniMonstro(JogoServidor *serv) {

	Coordenada resM;

	int i = 0;
	int r = 0;

	int x = 0;
	int y = 0;
	int cnt = 0;
	int tipo = 0;
	int prob = 0;

	resM.x = -1;
	resM.y = -1;

	for (r = 0; r < serv->mapa.tamsalas; r++) {

		x = serv->mapa.salas[r].porta.x;
		y = serv->mapa.salas[r].porta.y;


		cnt++;

		prob = aleatorio(1, 100, y*cnt);


		if (prob < 48 && serv->mapa.celula[x][y].monstro == -1
			&& !(existeJogadorNaPosicao(serv, x, y))
			) {
			serv->mapa.celula[x][y].monstro = -2;
			resM.x = x;
			resM.y = y;
			break;
		}
	}

	return resM;

}




int criaMonstro(JogoServidor *serv, int tipo, int energia, int n, int dup) {

	int x = 0, y = 0, r = 0;
	int tr = 0;
	int i = 0;

	Coordenada pos;
	pos.x = -1;
	pos.y = -1;

	if (serv->monstrosCriados >= MAXINIMIGOS)
		return -1;

	i = serv->monstrosCriados;

	switch (tipo)
	{

	case DISTRAIDO:
		swprintf(serv->monstros[i].descricao, 10, TEXT("%s"), "Distraido");
		serv->monstros[i].lentidao = LENTIDAO_MONSTRO_DIST;
		break;
	case BULLY:

		swprintf(serv->monstros[i].descricao, 10, TEXT("%s"), "Bully");
		serv->monstros[i].lentidao = LENTIDAO_MONSTRO_BULLY;

		break;
	default:
		break;
	}

	if (dup>=0)
		pos = monstroPosicaoAdjLivre(serv, dup);
	else
		pos = PosicaoIniMonstro(serv);


	serv->monstros[i].id = i;
	serv->monstros[i].posicao = pos;
	serv->monstros[i].tipo = tipo;
	serv->monstros[i].n_casas = n;
	serv->monstros[i].contadorMovimento = serv->monstros[i].lentidao;
	serv->monstros[i].energia = energia;

	serv->monstrosCriados++;

	return i;

}



void AtualizaMapaMonstro(JogoServidor *jogo, int mid, int ox, int oy) {

	int x = 0, y = 0;
	int r = 0;


	for (x = 0; x < jogo->mapa.tamx; x++)
	{
		for (y = 0; y < jogo->mapa.tamy; y++)
		{

			if (x == ox && y == oy && jogo->mapa.celula[x][y].monstro == mid) {
				jogo->mapa.celula[x][y].tipoMonstro = -1;
				jogo->mapa.celula[x][y].monstro = -1;
			}

			if (x == jogo->monstros[mid].posicao.x && y == jogo->monstros[mid].posicao.y) {
				jogo->mapa.celula[x][y].tipoMonstro = jogo->monstros[mid].tipo;
				jogo->mapa.celula[x][y].monstro = mid;
			}

		}
	}


}

