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

int existeJogadorNaPosicao(JogoServidor jogo, int  x, int y) {


	int i = 0;

	for (i = 0; i < jogo.jogadoresLigados; i++) {

		if (jogo.jogadores[i].posicao.x == x && jogo.jogadores[i].posicao.y == y)
			return 1;
	}

	return 0;

}


Item *devolveObjectoNaPosicao(JogoServidor jogo, int  x, int y) {


	int i = 0;

	for (i = 0; i < MAXVITAMINAS; i++) {

		if (jogo.vitaminas[i].posseJogador == 0)
			if (jogo.vitaminas[i].pos.x == x && jogo.vitaminas[i].pos.y == y)
				return &(jogo.vitaminas[i]);
	}

	return NULL;

}

void inicializaObjectos(JogoServidor *jog) {

	int i = 0;

	for (i = 0; i < MAXVITAMINAS; i++)
	{
		jog->vitaminas[i].tipo = 1;
		jog->vitaminas[i].posseJogador = 0;
		jog->vitaminas[i].qtdAtual = 0;
		jog->vitaminas[i].qtdMaxima = 200;
		jog->vitaminas[i].pos.x = 0;
		jog->vitaminas[i].pos.y = 0;
	}


	for (i = 0; i < MAXORANGES; i++)
	{

		jog->oranges[i].tipo = 2;
		jog->oranges[i].posseJogador = 0;
		jog->oranges[i].qtdAtual = 0;
		jog->oranges[i].qtdMaxima = 100;
		jog->oranges[i].pos.x = 0;
		jog->oranges[i].pos.y = 0;
	}



	for (i = 0; i < MAXCAFEINAS; i++)
	{
		jog->cafeinas[i].tipo = 3;
		jog->cafeinas[i].posseJogador = 0;
		jog->cafeinas[i].qtdAtual = 0;
		jog->cafeinas[i].qtdMaxima = 0;
		jog->cafeinas[i].pos.x = 0;
		jog->cafeinas[i].pos.y = 0;
	}



	for (i = 0; i < MAXPEDRAS; i++)
	{
		jog->pedras[i].tipo = 3;
		jog->pedras[i].posseJogador = 0;
		jog->pedras[i].qtdAtual = 0;
		jog->pedras[i].qtdMaxima = 15;
		jog->pedras[i].pos.x = 0;
		jog->pedras[i].pos.y = 0;
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

	for (r = 0; r < jog->mapa.tamsalas; r++) {

		x = jog->mapa.salas[r].porta.x;
		y = jog->mapa.salas[r].porta.y;

		if (!existeJogadorNaPosicao(*jog, x, y))
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

	for (x = x1; x < x1 + serv->jogoVisivel - 1; x++)
	{
		
		for (y = y1; y < y1 + serv->jogoVisivel - 1; y++)
		{
			i = (x1 + serv->jogoVisivel) - x;
			j = (y1 + serv->jogoVisivel) - y;

			jogcl->mapa[i][j] = serv->mapa.celula[x][y];
		}
	}


}


void criaJogo(JogoServidor *jog)
{


	inicializaObjectos(jog);

	jog->jogoVisivel = 15;

	Labirinto *lab = CriaLabirinto(200, 200, 10);

	jog->mapa = *lab;


}


void criaJogador(JogoServidor *jogo, TCHAR nome[], DWORD pid) {

	int i = 0;
	int j = 0;




	jogo->jogadores[jogo->jogadoresLigados].vitamina.pos.x = 0;
	jogo->jogadores[jogo->jogadoresLigados].vitamina.pos.y = 0;
	jogo->jogadores[jogo->jogadoresLigados].cafeina.pos.x = 0;
	jogo->jogadores[jogo->jogadoresLigados].cafeina.pos.y = 0;
	jogo->jogadores[jogo->jogadoresLigados].pedra.pos.x = 0;
	jogo->jogadores[jogo->jogadoresLigados].pedra.pos.y = 0;
	jogo->jogadores[jogo->jogadoresLigados].orange.pos.x = 0;
	jogo->jogadores[jogo->jogadoresLigados].orange.pos.y = 0;

	jogo->jogadores[jogo->jogadoresLigados].vitamina.posseJogador = 0;
	jogo->jogadores[jogo->jogadoresLigados].vitamina.qtdAtual = 0;
	jogo->jogadores[jogo->jogadoresLigados].vitamina.qtdMaxima = 0;
	jogo->jogadores[jogo->jogadoresLigados].vitamina.tipo = 1;

	jogo->jogadores[jogo->jogadoresLigados].cafeina.posseJogador = 0;
	jogo->jogadores[jogo->jogadoresLigados].cafeina.qtdAtual = 0;
	jogo->jogadores[jogo->jogadoresLigados].cafeina.qtdMaxima = 0;
	jogo->jogadores[jogo->jogadoresLigados].cafeina.tipo = 2;

	jogo->jogadores[jogo->jogadoresLigados].orange.posseJogador = 0;
	jogo->jogadores[jogo->jogadoresLigados].orange.qtdAtual = 0;
	jogo->jogadores[jogo->jogadoresLigados].orange.qtdMaxima = 0;
	jogo->jogadores[jogo->jogadoresLigados].orange.tipo = 3;

	jogo->jogadores[jogo->jogadoresLigados].pedra.posseJogador = 0;
	jogo->jogadores[jogo->jogadoresLigados].pedra.qtdAtual = 0;
	jogo->jogadores[jogo->jogadoresLigados].pedra.qtdMaxima = 0;
	jogo->jogadores[jogo->jogadoresLigados].pedra.tipo = 4;

	jogo->jogadores[jogo->jogadoresLigados].lentidao = 0;
	jogo->jogadores[jogo->jogadoresLigados].saude = 0;

	Coordenada pos = PosicaoIniJog(jogo);

	jogo->jogadores[jogo->jogadoresLigados].posicao.x = pos.x;
	jogo->jogadores[jogo->jogadoresLigados].posicao.y = pos.y;


	_tcscpy_s(jogo->jogadores[jogo->jogadoresLigados].nome, 30, nome);


}

