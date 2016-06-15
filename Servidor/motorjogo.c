#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\memoria.h"

#include "JogoServidor.h"
#include "motorjogo.h"


int JogadoresOnline(JogoServidor *jogo) {

	int i = 0;
	int cnt = 0;
	for (i = 0; i < jogo->jogadoresLigados; i++)
	{

		if (jogo->clientes[i].jogo.id >= 0)
		{
			cnt++;
		}
	}

	return cnt;
}

int validaMovimentoBase(JogoServidor *serv, int x, int y) {




	if ((
		serv->mapa.celula[x][y].tipo == TipoCelula_CHAO ||
		serv->mapa.celula[x][y].tipo == TipoCelula_PORTA))
		return 1; else return 0;


}

void atualizaJogadorCliente(JogoServidor *serv, JogoCliente* jogcl) {

	serv->clientes[jogcl->id].jogo.jogador.posicao = jogcl->jogador.posicao;
	serv->clientes[jogcl->id].jogo.comando = jogcl->comando;
	serv->clientes[jogcl->id].jogo.moveuDirecao = jogcl->moveuDirecao;
	serv->clientes[jogcl->id].jogo.usarPedra = jogcl->usarPedra;
}

void forcaDadosServidor(JogoServidor *serv, JogoCliente* jogcl) {

	jogcl->jogador.efeitoCafeina = serv->clientes[jogcl->id].jogo.jogador.efeitoCafeina;
	jogcl->jogador.saude = serv->clientes[jogcl->id].jogo.jogador.saude;
	jogcl->jogador.lentidao = serv->clientes[jogcl->id].jogo.jogador.lentidao;
	jogcl->jogador.qtdPedras = serv->clientes[jogcl->id].jogo.jogador.qtdPedras;
	jogcl->jogador.contadorMovimento = serv->clientes[jogcl->id].jogo.jogador.contadorMovimento;



}


int validaMovimentoJogador(JogoServidor *serv, JogoCliente *jog, int nx, int ny) {

	if (!validaMovimentoBase(serv, nx, ny))
		return 0;

	if (existeJogadorNaPosicao(serv, nx, ny))
		return 0;

	return 1;
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

int jogadorPosicaoAdj(JogoServidor *jogo, int id, int  x, int y) {


	int i = 0;

	for (i = 0; i < jogo->jogadoresLigados; i++) {

		if (jogo->clientes[i].jogo.id >= 0)
		{

			if (id != i)
				if (
					(jogo->clientes[i].jogo.jogador.posicao.x - 1 == x &&
						y == jogo->clientes[i].jogo.jogador.posicao.y - 1)
					|| (jogo->clientes[i].jogo.jogador.posicao.x == x &&
						y == jogo->clientes[i].jogo.jogador.posicao.y - 1)
					|| (jogo->clientes[i].jogo.jogador.posicao.x + 1 == x &&
						y == jogo->clientes[i].jogo.jogador.posicao.y - 1)
					|| (jogo->clientes[i].jogo.jogador.posicao.x - 1 == x &&
						y == jogo->clientes[i].jogo.jogador.posicao.y)
					|| (jogo->clientes[i].jogo.jogador.posicao.x + 1 == x &&
						y == jogo->clientes[i].jogo.jogador.posicao.y)
					|| (jogo->clientes[i].jogo.jogador.posicao.x - 1 == x &&
						y == jogo->clientes[i].jogo.jogador.posicao.y + 1)
					|| (jogo->clientes[i].jogo.jogador.posicao.x == x &&
						y == jogo->clientes[i].jogo.jogador.posicao.y + 1)
					|| (jogo->clientes[i].jogo.jogador.posicao.x + 1 == x &&
						y == jogo->clientes[i].jogo.jogador.posicao.y + 1)
					)
					return  i;
		}
	}


	return -1;
}

void combateJogvsJog(JogoServidor *jogo, int p1, int p2) {


	int i = 0;

	if (jogo->clientes[p1].jogo.id >= 0)
	{

		if (jogo->clientes[p1].jogo.usarPedra == 1) {


			if (jogo->clientes[p2].jogo.jogador.saude - 2 <= 0)
				jogo->clientes[p2].jogo.jogador.saude = 0;
			else jogo->clientes[p2].jogo.jogador.saude -= 2;


			jogo->clientes[p2].jogo.usarPedra = 0;

		}
		else
			if (jogo->clientes[p2].jogo.jogador.saude - 1 <= 0)
				jogo->clientes[p2].jogo.jogador.saude = 0;
			else jogo->clientes[p2].jogo.jogador.saude -= 1;


	}

}


void atualizaPosicao(JogoServidor *serv, JogoCliente *jogcl, int px, int py) {

	int x = 0;
	int y = 0;
	int en = -1;

	for (x = 0; x < serv->mapa.tamx; x++)
	{

		for (y = 0; y < serv->mapa.tamy; y++)
		{

			if (x == px && y == py)
			{

				en = jogadorPosicaoAdj(serv, jogcl->id, px, py);
				//combate
				if (en >= 0)
					combateJogvsJog(serv, jogcl->id, en);

				//apanha objectos
				switch (serv->mapa.celula[x][y].objeto)
				{
				case Tipo_Pedra:

					//apanha pedra se puder carregar
					if (jogcl->jogador.qtdPedras + 1 <= MAXPEDRAS) {

						jogcl->jogador.qtdPedras++;
						serv->mapa.celula[x][y].objeto = 0;

					}

					break;

				case Tipo_SacoPedra:

					//apanha pedra se puder carregar
					if (jogcl->jogador.qtdPedras + 5 <= MAXPEDRAS) {

						jogcl->jogador.qtdPedras += 5;
						serv->mapa.celula[x][y].objeto = 0;

					}

					break;

				case Tipo_Vitamina:

					if ((jogcl->jogador.saude + 1) <= (SAUDE_JOG_INI * 2))
						jogcl->jogador.saude++;

					serv->mapa.celula[x][y].objeto = 0;

					break;

				case Tipo_Pocao:


					jogcl->jogador.saude = SAUDE_JOG_INI;

					serv->mapa.celula[x][y].objeto = 0;

					break;

				case Tipo_Cafeina:

					if (jogcl->jogador.efeitoCafeina == 0) {

						jogcl->jogador.lentidao -= 2;

						if (jogcl->jogador.lentidao <= 1) jogcl->jogador.lentidao = 1;

						jogcl->jogador.efeitoCafeina = 1;

						serv->mapa.celula[x][y].objeto = 0;
					}

					break;

				case Tipo_OrangeBull:


					if ((jogcl->jogador.saude + 3) <= (SAUDE_JOG_INI * 2))
						jogcl->jogador.saude += 3;

					serv->mapa.celula[x][y].objeto = 0;

					break;

				default:
					break;
				}
				//fim apanhar objectos

			}
		}
	}


}

//Copia mapa Origem / Destino 
void atualizaMapaEntreClientes(JogoCliente *origem, JogoCliente *dest) {

	int x = 0;
	int y = 0;



	for (x = 0; x < MAXVISX; x++)
	{

		for (y = 0; y < MAXVISY; y++)
		{
			dest->mapa[x][y] = origem->mapa[x][y];
		}
	}

}

void inicializaObjectos(JogoServidor *jog) {


	int x = 0;
	int y = 0;
	int r = 0;


	for (x = 0; x < jog->mapa.tamx; x++)
	{

		for (y = 0; y < jog->mapa.tamy; y++)
		{

			jog->mapa.celula[x][y].objeto = 0;
			jog->mapa.celula[x][y].jogador = 0;
			jog->mapa.celula[x][y].monstro = -1;
			jog->mapa.celula[x][y].tipoMonstro = -1;

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

		if (!existeJogadorNaPosicao(jog, x, y)
			&& jog->mapa.celula[x][y].monstro == -1
			)
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

			jogcl->mapa[i][j] = serv->mapa.celula[x][y];

		}
	}

}



void atualizaMapaServidor(JogoServidor *serv, JogoCliente *jogcl, int oldx, int oldy) {

	int x = 0;
	int y = 0;

	int r = 0;


	for (x = 0; x < serv->mapa.tamx; x++)
	{

		for (y = 0; y < serv->mapa.tamy; y++)
		{

			serv->mapa.celula[x][y].jogador = 0;


			for (r = 0; r < serv->jogadoresLigados; r++)
			{
				if (serv->clientes[r].jogo.id >= 0)
				{

					if (serv->clientes[r].jogo.pidCliente != jogcl->pidCliente) {
						if (x == serv->clientes[r].jogo.jogador.posicao.x && y == serv->clientes[r].jogo.jogador.posicao.y)
							serv->mapa.celula[x][y].jogador = serv->clientes[r].jogo.pidCliente;
					}

					if (serv->clientes[r].jogo.pidCliente == jogcl->pidCliente) {
						if (x == jogcl->jogador.posicao.x && y == jogcl->jogador.posicao.y)
							serv->mapa.celula[x][y].jogador = serv->clientes[r].jogo.pidCliente;
					}
				}
			}

		}
	}

}



void criaObjectosMapa(JogoServidor *serv) {


	int tipo = 0;
	int cnt = 0;
	int x = 0;
	int y = 0;
	int prob = 0;
	int r = 0;
	int cp = 0;

	for (int i = 0; i < MAXOBJECTOS; i++) {

		for (x = 0; x < serv->mapa.tamx; x++)
		{

			for (y = 0; y < serv->mapa.tamy; y++)
			{


				if (serv->mapa.celula[x][y].tipo == TipoCelula_CHAO) {


					r = rand() % 100;

					if (r < PERC_VITAMINAS) tipo = Tipo_Vitamina;
					else if (r >= PERC_VITAMINAS && r < PERC_ORANGE + PERC_VITAMINAS)
						tipo = Tipo_OrangeBull;

					else if (r >= PERC_ORANGE + PERC_VITAMINAS &&
						r < PERC_ORANGE + PERC_VITAMINAS + PERC_PEDRA)
						tipo = Tipo_Pedra;

					else if (r >= PERC_ORANGE + PERC_VITAMINAS &&
						r < PERC_ORANGE + PERC_PEDRA / 4)
						tipo = Tipo_SacoPedra;

					else if (r < PERC_VITAMINAS && cp )
						tipo = Tipo_Pocao;

					else
						tipo = Tipo_Cafeina;

					cnt++;
					prob = aleatorio(1, 100, y*cnt);

					if (prob < 8) {
						serv->mapa.celula[x][y].objeto = tipo;

					}

				}

			}
		}
	}

}

void criaJogo(JogoServidor *jog)
{

	jog->mapa = *CriaLabirinto(200, 200, 10);


	inicializaObjectos(jog);
	criaObjectosMapa(jog);


	/*FILE *fp;

	fp = fopen("bd.dat", "wb");

	if (fp == NULL)
	{
		return 0;
	}


	fwrite(&jog->mapa, sizeof(Labirinto), 1, fp);


	fclose(fp);
*/

}

int carregaLabirintoDefeito(JogoServidor *jog) {

	TCHAR Buffer[256];
	TCHAR file[256];
	DWORD dwRet;
	FILE *fp;
	Labirinto lab;


	dwRet = GetCurrentDirectory(256, Buffer);

	swprintf(file, 256, TEXT("%s\\lab.dat"), Buffer);

	fp = _tfopen(file, TEXT("rb"));

	if (fp == NULL)
	{
		return -1;
	}


	while (fread(&lab, sizeof(Labirinto), 1, fp) == 1);

	jog->mapa = lab;

	fclose(fp);

	return 1;

}

void criaJogador(JogoServidor *jogo, JogoCliente *clt) {

	int i = 0;
	int j = 0;


	clt->usarPedra = 0;
	clt->jogador.efeitoCafeina = 0;


	clt->jogador.qtdPedras = 0;


	clt->jogador.saude = SAUDE_JOG_INI;
	clt->jogador.lentidao = LENTIDAO_JOG_INI;
	clt->jogador.contadorMovimento = LENTIDAO_JOG_INI;
	clt->jogador.podeMovimentar = 1;


	clt->jogador.pidJogador = 0;


	Coordenada pos = PosicaoIniJog(jogo);

	clt->jogador.posicao.x = pos.x;
	clt->jogador.posicao.y = pos.y;

	clt->jogador.pidJogador = clt->pidCliente;

	swprintf(clt->jogador.nome, 256, TEXT("Jogador %d"), jogo->jogadoresLigados + 1);

}
