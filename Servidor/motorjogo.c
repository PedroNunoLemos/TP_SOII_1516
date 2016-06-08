#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"

#include "JogoServidor.h"
#include "motorjogo.h"

int existeJogadorNaPosicao(JogoServidor *jogo, int  x, int y) {


	int i = 0;

	for (i = 0; i < jogo->jogadoresLigados; i++) {

		if (jogo->clientes[i].jogo.jogador.posicao.x == x && jogo->clientes[i].jogo.jogador.posicao.y == y)
			return 1;
	}

	return 0;

}

/*
Item *devolveObjectoNaPosicao(JogoServidor jogo, int  x, int y) {


	int i = 0;

	return jogo.objectos[x][y].objecto;

}*/

void inicializaObjectos(JogoServidor *jog) {


	int x = 0;
	int y = 0;
	int r = 0;


	for (x = 0; x < jog->mapa->tamx; x++)
	{

		for (y = 0; y < jog->mapa->tamy; y++)
		{


			jog->mapa->celula[x][y].objeto = 0;
			jog->mapa->celula[x][y].jogador = 0;


		}


	}

}



Coordenada PosicaoIniJog(JogoServidor *jog) {

	Coordenada res;

	int i = 0;
	int r = 0;

	int x = 0;
	int y = 0;

	res.x = -1;
	res.y = -1;

	for (r = 0; r < jog->mapa->tamsalas; r++) {

		x = jog->mapa->salas[r].porta.x;
		y = jog->mapa->salas[r].porta.y;

		if (!existeJogadorNaPosicao(jog, x, y))
		{
			res.x = x;
			res.y = y;

			return res;
		}


	}

	return res;
}

void atualizaMapaCliente(JogoServidor *serv, JogoCliente *jogcl, int x1, int y1) {

	int x = 0;
	int y = 0;
	int i = -1;
	int j = -1;
	int cntjog = 0;


	for (x = x1; x < x1 + MAXVISX + 1; x++)
	{

		for (y = y1; y < y1 + MAXVISY + 1; y++)
		{
			i = (x1 + MAXVISX) - x;
			j = (y1 + MAXVISY) - y;

			jogcl->mapa[i][j] = serv->mapa->celula[x][y];

		}
	}

}



void atualizaMapaServidor(JogoServidor *serv, JogoCliente *jogcl, int oldx, int oldy) {

	int x = 0;
	int y = 0;

	int r = 0;


	for (x = 0; x < serv->mapa->tamx; x++)
	{

		for (y = 0; y < serv->mapa->tamy; y++)
		{

			serv->mapa->celula[x][y].jogador = 0;

			for (r = 0; r < serv->jogadoresLigados; r++)
			{
				if (x == serv->clientes[r].jogo.jogador.posicao.x && y == serv->clientes[r].jogo.jogador.posicao.y)
				{
					serv->mapa->celula[x][y].jogador = serv->clientes[r].jogo.pidCliente;
				}
			}


		}
	}

}



void criaObjectosMapa(JogoServidor *serv) {


	int tipo = 0;


	int r = 0;


	int cnt = 0;
	int x = 0;
	int y = 0;
	int prob = 0;


	for (x = 0; x < serv->mapa->tamx; x++)
	{

		for (y = 0; y < serv->mapa->tamy; y++)
		{


			if (serv->mapa->celula[x][y].tipo == TipoCelula_CHAO) {

				cnt++;
				prob = aleatorio(1, 100, y*cnt);
				tipo = aleatorio(2, 8, prob);


				if (prob < 8) {
					serv->mapa->celula[x][y].objeto = tipo / 2;

				}

			}

		}
	}
}



void criaJogo(JogoServidor *jog)
{


	Labirinto *lab = CriaLabirinto(200, 200, 10);

	jog->mapa = lab;


	inicializaObjectos(jog);
	criaObjectosMapa(jog);


}


void criaJogador(JogoServidor *jogo, JogoCliente *clt) {

	int i = 0;
	int j = 0;



	clt->jogador.qtdCafeinas = 0;
	clt->jogador.qtdOranges = 0;
	clt->jogador.qtdPedras = 0;
	clt->jogador.qtdVitaminas = 0;

	clt->jogador.saude = 10;
	clt->jogador.lentidao = 5;

	clt->jogador.pidJogador = 0;


	Coordenada pos = PosicaoIniJog(jogo);

	clt->jogador.posicao.x = pos.x;
	clt->jogador.posicao.y = pos.y;

	clt->jogador.pidJogador = clt->pidCliente;


	swprintf(clt->jogador.nome, 256, TEXT("Jogador %d"), jogo->jogadoresLigados + 1);

}

