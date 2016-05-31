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

void atualizaJogadorServidor(JogoServidor *jog, JogoCliente *jogcl) {

	int i = 0;

	for (i = 0; i < jog->jogadoresLigados; i++)
	{
		if (jog->jogadores[i].pidJogador == jogcl->pidCliente) {

			jog->jogadores[i] = (jogcl->jogador);

			return;

		}

		if (jog->jogoClientes[i].pidCliente == jogcl->pidCliente) {

			jog->jogoClientes[i] = *jogcl;

			return;

		}



	}

	return;
}

int existeJogadorNaPosicao(JogoServidor *jogo, int  x, int y) {


	int i = 0;

	for (i = 0; i < jogo->jogadoresLigados; i++) {

		if (jogo->jogadores[i].posicao.x == x && jogo->jogadores[i].posicao.y == y)
			return 1;
	}

	return 0;

}


Item *devolveObjectoNaPosicao(JogoServidor jogo, int  x, int y) {


	int i = 0;

	return jogo.objectos[x][y].objecto;

}

void inicializaObjectos(JogoServidor *jog) {


	int x = 0;
	int y = 0;
	int r = 0;


	for (x = 0; x < jog->mapa.tamx; x++)
	{

		for (y = 0; y < jog->mapa.tamy; y++)
		{
			for (r = 0; r < 5; r++)
			{
				jog->objectos[x][y].objecto[r].tipo = 0;
			}


			jog->jogadoresMapa[x][y].posicaoOcupada = 0;
			jog->jogadoresMapa[x][y].pidCliente = 0;


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

	for (r = 0; r < jog->mapa.tamsalas; r++) {

		x = jog->mapa.salas[r].porta.x;
		y = jog->mapa.salas[r].porta.y;

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


	for (x = x1; x < x1 + serv->jogoVisivel; x++)
	{

		for (y = y1; y < y1 + serv->jogoVisivel; y++)
		{
			i = (x1 + serv->jogoVisivel - 1) - x;
			j = (y1 + serv->jogoVisivel - 1) - y;

			jogcl->mapa[i][j] = serv->mapa.celula[x][y];
			jogcl->objectosMapa[i][j] = serv->objectos[x][y];
			jogcl->jogadoresMapa[i][j] = serv->jogadoresMapa[x][y];

		}
	}

}



void atualizaMapaServidor(JogoServidor *serv, JogoCliente *jogcl, int oldx, int oldy) {

	int x = 0;
	int y = 0;
	int i = -1;
	int j = -1;
	int r = 0;


	for (x = 0; x < serv->mapa.tamx; x++)
	{

		for (y = 0; y < serv->mapa.tamy; y++)
		{

			if (x == oldx && y == oldy)
			{

				if (serv->jogadoresMapa[x][y].posicaoOcupada == jogcl->pidCliente) {
					serv->jogadoresMapa[x][y].posicaoOcupada = 0;
					serv->jogadoresMapa[x][y].pidCliente = 0;

				}
			}

			if (x == jogcl->jogador.posicao.x && y == jogcl->jogador.posicao.y)
			{

				if (serv->jogadoresMapa[x][y].posicaoOcupada == 0) {
					serv->jogadoresMapa[x][y].posicaoOcupada = jogcl->pidCliente;
					serv->jogadoresMapa[x][y].pidCliente = jogcl->pidCliente;
					break;
				}

			}


		}
	}

}



void criaObjectosMapa(JogoServidor *serv) {

	int xx = 0;
	int yy = 0;

	int tipo = 0;

	int sx = 0;
	int sy = 0;

	int r = 0;

	for (r = 0; r < serv->mapa.tamsalas; r++) {

		Sala sala = serv->mapa.salas[r];

		for (xx = sala.x + 1; xx < sala.x + sala.w - 1; xx++) {
			for (yy = sala.y + 1; yy < sala.y + sala.h - 1; yy++) {

				tipo = aleatorio(0, 4, yy);



				sx = aleatorio(sala.x + 1, sala.x + sala.w - 1, xx);
				sy = aleatorio(sala.y + 1, sala.y + sala.h - 1, yy);


				serv->objectos[sx][sy].objecto[0].tipo = tipo;

			}
		}

	}

}



void criaJogo(JogoServidor *jog)
{

	jog->jogoVisivel = 15;

	Labirinto *lab = CriaLabirinto(200, 200, 10);

	jog->mapa = *lab;


	inicializaObjectos(jog);
	criaObjectosMapa(jog);


}


void criaJogador(JogoServidor *jogo, TCHAR nome[], DWORD pid) {

	int i = 0;
	int j = 0;




	jogo->jogadores[jogo->jogadoresLigados].qtdCafeinas = 0;
	jogo->jogadores[jogo->jogadoresLigados].qtdOranges = 0;
	jogo->jogadores[jogo->jogadoresLigados].qtdPedras = 0;
	jogo->jogadores[jogo->jogadoresLigados].qtdVitaminas = 0;

	jogo->jogadores[jogo->jogadoresLigados].saude = 10;
	jogo->jogadores[jogo->jogadoresLigados].lentidao = 5;


	Coordenada pos = PosicaoIniJog(jogo);

	jogo->jogadores[jogo->jogadoresLigados].posicao.x = pos.x;
	jogo->jogadores[jogo->jogadoresLigados].posicao.y = pos.y;

	jogo->jogoClientes[jogo->jogadoresLigados].pidCliente = pid;

	swprintf(jogo->jogadores[jogo->jogadoresLigados].nome, 256, TEXT("Jogador %d"), jogo->jogadoresLigados + 1);




}

