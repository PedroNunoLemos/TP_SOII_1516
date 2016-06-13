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





Coordenada monstroPosicaoMovAdjLivre(JogoServidor *jogo, int mid, int dir) {

	int i = 0;
	int x = 0;
	int y = 0;

	Coordenada res;

	Coordenada mp = jogo->monstros[mid].posicao;

	res.x = -1;
	res.y = -1;

	for (x = 0; x < jogo->mapa.tamx; x++)
	{
		for (y = 0; y < jogo->mapa.tamy; y++)
		{

			Celula tc = jogo->mapa.celula[x][y];

			if (mp.x == x && mp.y == y) {

				if (celulaVaziaMapa(jogo, x, y - 1) && dir == 1) //cima
				{
					res.x = x;
					res.y = y - 1;

					return res;

				}


				if (celulaVaziaMapa(jogo, x - 1, y) && dir == 2) // esquerda
				{
					res.x = x - 1;
					res.y = y;

					return res;

				}

				if (celulaVaziaMapa(jogo, x, y + 1) && dir == 3) // baixo
				{
					res.x = x;
					res.y = y + 1;

					return res;

				}

				if (celulaVaziaMapa(jogo, x + 1, y) && dir == 4) // direita
				{
					res.x = x + 1;
					res.y = y;

					return res;

				}



			}
		}
	}

	return res;

}




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

int celulaVaziaMapa(JogoServidor *jogo, int tx, int ty) {

	int res = 0;

	Celula lc = jogo->mapa.celula[tx][ty];

	res = (lc.tipo == TipoCelula_CHAO || lc.tipo == TipoCelula_PORTA);

	return  res;

}

void AndarAoCalhas(JogoServidor *jogo, int mid) {

	int dir = 0;
	int odir = -1;
	int tipo = 0;
	int cnt = 0;
	int i = 0;

	int prob = 0;

	Coordenada adj;

	adj.x = -1;
	adj.y = -1;

	cnt = 0;
	//escolhemos uma direção aleatoria
	do {

		dir = aleatorio(1, 4, GetCurrentTime());

		adj = monstroPosicaoMovAdjLivre(jogo, mid, dir);

		cnt++;

	} while ((adj.x == -1 || adj.y == -1) && cnt < 4);

	// não ha celulas livres aguardamos
	if (cnt == 4)
		return;

	//movimento n_casas numa direção
	for (i = 0; i < jogo->monstros[mid].n_casas; i++)
	{

		//   N	     W	   S	  E	
		if (dir == 1 && celulaVaziaMapa(jogo, adj.x, adj.y - 1) == 1) adj.y--;
		if (dir == 2 && celulaVaziaMapa(jogo, adj.x - 1, adj.y) == 1) adj.x--;
		if (dir == 3 && celulaVaziaMapa(jogo, adj.x, adj.y + 1) == 1) adj.y++;
		if (dir == 4 && celulaVaziaMapa(jogo, adj.x + 1, adj.y) == 1) adj.x++;


		jogo->monstros[mid].posicao.x = adj.x;
		jogo->monstros[mid].posicao.y = adj.y;


	}

	prob = aleatorio(1, 100, GetCurrentTime());

	if (prob < 75) {

		//guardamos o ultimo sentido do movimento
		odir = dir;

		cnt = 0;
		//escolhemos uma direção aleatoria
		do {

			dir = aleatorio(1, 4, GetCurrentTime());

			//   N	     W	   S	  E	
			if (dir == 1 && celulaVaziaMapa(jogo, adj.x, adj.y - 1) == 1) adj.y--;
			if (dir == 2 && celulaVaziaMapa(jogo, adj.x - 1, adj.y) == 1) adj.x--;
			if (dir == 3 && celulaVaziaMapa(jogo, adj.x, adj.y + 1) == 1) adj.y++;
			if (dir == 4 && celulaVaziaMapa(jogo, adj.x + 1, adj.y) == 1) adj.x++;


			cnt++;

		} while (odir == dir);


		// não ha celulas livres aguardamos
		if (cnt == 4)
			return;

		jogo->monstros[mid].posicao = adj;


	} // fim prob mudar de direção



}
int PersegueJogador(JogoServidor *jogo, int mid) {

	int res = 0;

	int x = 0;
	int y = 0;


	for (x = jogo->monstros[mid].posicao.x - VISAO_MONSTRO; x < jogo->monstros[mid].posicao.x + VISAO_MONSTRO; x++)
	{
		for (y = jogo->monstros[mid].posicao.y - VISAO_MONSTRO; y < jogo->monstros[mid].posicao.y + VISAO_MONSTRO; y++)
		{
			if (existeJogadorNaPosicao(jogo, x, y)) {

				res = 1;

				if (jogo->monstros[mid].posicao.x > x && celulaVaziaMapa(jogo, x - 1, y)) { jogo->monstros[mid].posicao.x--; continue; }
				if (jogo->monstros[mid].posicao.x < x && celulaVaziaMapa(jogo, x + 1, y)) { jogo->monstros[mid].posicao.x++; continue; }
				if (jogo->monstros[mid].posicao.y > y && celulaVaziaMapa(jogo, x, y - 1)) { jogo->monstros[mid].posicao.y--; continue; }
				if (jogo->monstros[mid].posicao.y < y && celulaVaziaMapa(jogo, x, y + 1)) { jogo->monstros[mid].posicao.y++; continue; }

			}

		}
	}

	return res;
}

void MovimentaMonstro(JogoServidor *jogo, int mid) {

	if (mid < jogo->monstrosCriados)
	{
		if (jogo->monstros[mid].tipo == BULLY) {

			if (!PersegueJogador(jogo, mid)) {
				AndarAoCalhas(jogo, mid);
			}


		}

		if (jogo->monstros[mid].tipo == DISTRAIDO) {

			AndarAoCalhas(jogo, mid);

		} // fim dist

	}

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

	if (dup >= 0)
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

